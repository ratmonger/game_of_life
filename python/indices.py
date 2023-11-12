def get_neighbor_indices(idx: n, grid_shape: tuple[int, int]) -> list[int]:
    """
    Sample implementation for computing neighbor indices
    """
    rows, cols = grid_shape

    neighbors = []

    on_left_edge = idx % cols == 0
    on_right_edge = idx % cols == (cols - 1)
    on_top_edge = idx < cols
    on_bottom_edge = (idx // cols == rows - 1)

    if not on_left_edge:
        neighbors.append(idx - 1)
    if not on_right_edge:
        neighbors.append(idx + 1)
    if not on_top_edge:
        neighbors.append(idx - cols)
    if not on_bottom_edge:
        neighbors.append(idx + cols)

    # diagonal neighbors
    if (not on_left_edge) and (not on_top_edge):
        neighbors.append(idx - cols - 1)
    if (not on_left_edge) and (not on_bottom_edge):
        neighbors.append(idx + cols - 1)
    if (not on_right_edge) and (not on_top_edge):
        neighbors.append(idx - cols + 1)
    if (not on_right_edge) and (not on_bottom_edge):
        neighbors.append(idx + cols + 1)

    return neigbors
