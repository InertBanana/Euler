#include "utils.h"

int problem_11();
void search(std::array<std::array<int, 20>, 20>& mat, int x_start, int y_start, int x_lim, int y_lim, int x_dir, int y_dir, std::promise<int> * prom);
