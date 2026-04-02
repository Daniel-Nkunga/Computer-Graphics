#include <vector>
#include <tuple>

using namespace std;

// Source - https://stackoverflow.com/a/2076668
// Posted by jamesdlin, modified by community. See post 'Timeline' for change history
// Retrieved 2026-04-01, License - CC BY-SA 4.0

class Matrix
{
public:
    Matrix(size_t rows, size_t cols);
    double& operator()(size_t i, size_t j);
    double operator()(size_t i, size_t j) const;

private:
    size_t mRows;
    size_t mCols;
    std::vector<double> mData;
};

Matrix::Matrix(size_t rows, size_t cols)
: mRows(rows),
  mCols(cols),
  mData(rows * cols)
{
}

double& Matrix::operator()(size_t i, size_t j)
{
    return mData[i * mCols + j];
}

double Matrix::operator()(size_t i, size_t j) const
{
    return mData[i * mCols + j];
}tuple<int, int> dimension()