import numpy as np
from scipy import sparse
import matplotlib.pyplot as plt
from matplotlib import animation


NEIGHBOR_STENCIL = np.ones((3,3), dtype=np.uint64)
NEIGHBOR_STENCIL[1,1] = 0
# UPDATE_FUNC = np.vectorize(lambda n: 1 if (n > 1 and n < 4) else 0)

def make_poisson_mtx(stencil: np.ndarray, grid_shape: tuple[int, int]) -> np.ndarray:
    # assume a 3x3 stencil and a square grid
    num_cells = np.prod(grid_shape)
    num_nonzero = (stencil != 0).sum()
    diags = np.zeros(num_nonzero, dtype=np.uint64)

    # compute index offset of each dof within the stencil
    strides = np.cumprod([1] + list(grid_shape), dtype=np.uint64)[:-1]
    indices = tuple(i.copy() for i in stencil.nonzero())
    for i, s in zip(indices, stencil.shape):
        i -= s // 2
    for stride, coords in zip(strides, reversed(indices)):
        diags = np.add(diags, stride * coords)

    data = stencil[stencil != 0].repeat(num_cells).reshape(num_nonzero, num_cells)

    indices = np.vstack(indices).T

    # zero boundary connections
    for index, diag in zip(indices, data):
        diag = diag.reshape(grid_shape)
        for n, i in enumerate(index):
            if i > 0:
                s = [slice(None)] * len(grid_shape)
                s[n] = slice(0, i)
                s = tuple(s)
                diag[s] = 0
            elif i < 0:
                s = [slice(None)]*len(grid_shape)
                s[n] = slice(i, None)
                s = tuple(s)
                diag[s] = 0

    # remove diagonals that lie outside matrix
    mask = abs(diags) < num_cells
    if not mask.all():
        diags = diags[mask]
        data = data[mask]

    # sum duplicate diagonals
    if len(np.unique(diags)) != len(diags):
        new_diags = np.unique(diags)
        new_data = np.zeros((len(new_diags), data.shape[1]),
                            dtype=data.dtype)

        for dia, dat in zip(diags, data):
            n = np.searchsorted(new_diags, dia)
            new_data[n, :] += dat

        diags = new_diags
        data = new_data

    return sparse.dia_matrix((data, diags),
                             shape=(num_cells, num_cells), dtype=np.uint8)


def make_glider() -> np.ndarray:
    return np.array([[0, 1, 0], [0, 0, 1], [1, 1, 1]], dtype=np.uint8)


def embed(arr: np.ndarray, grid_width: int) -> np.ndarray:
    grid = np.zeros((grid_width, grid_width), dtype=np.uint8)

    for i in range(arr.shape[0]):
        for j in range(arr.shape[1]):
            grid[i,j] = arr[i,j]

    return grid.flatten()


def run(init_state: np.ndarray, ax, max_iter: int = 10) -> np.ndarray:
    grid_width = int(np.sqrt(init_state.size))
    grid = init_state
    mtx = make_poisson_mtx(NEIGHBOR_STENCIL, (grid_width, grid_width))

    ims = []
    im = ax.imshow(grid.reshape((grid_width, grid_width)), cmap='gray_r', animated=True)
    ims.append([im])

    for i in range(max_iter):
        neighbors = mtx @ grid
        grid = (neighbors == 3) | (grid & (neighbors == 2))
        im = ax.imshow(grid.reshape((grid_width, grid_width)), cmap='gray_r', animated=True)
        ims.append([im])

    return ims


if __name__ == '__main__':
    # grid = np.random.choice([True] + [False] * 249, grid_width**2)
    grid = embed(make_glider(), 50)
    fig, ax = plt.subplots()
    result = run(grid, ax, max_iter=100)

    ani = animation.ArtistAnimation(fig, result, interval=100, blit=True, repeat_delay=1000)
    ani.save('gol.mp4')
    # plt.show()

