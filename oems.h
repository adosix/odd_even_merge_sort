
// process_rank, max_destination, max_tag, min_destination, min_tag
int merging_scheme[19][5] = {0, 4, 0, 5, 0,
                             1, 4, 1, 5, 1,
                             2, 6, 0, 7, 0,
                             3, 6, 1, 7, 1,
                             4, 10, 0, 8, 0,
                             5, 8, 1, 13, 0,
                             6, 10, 1, 9, 0,
                             7, 9, 1, 13, 1,
                             8, 12, 0, 11, 0,
                             9, 12, 1, 11, 1,
                             10, 0, 0, 14, 0,
                             11, 14, 1, 18, 0,
                             12, 16, 0, 15, 0,
                             13, 15, 1, 0, 7,
                             14, 16, 1, 17, 0,
                             15, 17, 1, 18, 1,
                             16, 0, 1, 0, 2,
                             17, 0, 3, 0, 4,
                             18, 0, 5, 0, 6};