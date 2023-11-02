import numpy as np
from scipy import sparse

NEIGHBOR_STENCIL = np.ones((3,3), dtype=np.uint64)
NEIGHBOR_STENCIL[1,1] = 0

def make_poisson_mtx(stencil: np.ndarray, grid_shape: tuple[int, int]) -> np.ndarray:
    # assume a square stencil
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
                             shape=(num_cells, num_cells))

if __name__ == '__main__':
    mtx = make_poisson_mtx(NEIGHBOR_STENCIL, (3, 3))
    print(mtx.toarray())
    print(mtx.dtype)
    print((mtx @ NEIGHBOR_STENCIL.flatten()).reshape(3, 3))
