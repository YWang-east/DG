#pragma once

#include <cassert>
#include <cstddef>
#include "vector.hpp"

namespace DG::core{
    /** 
     * @brief data type: matrix
     * 
     * derived class from `vec` to provide a matrix-like interface
     */
    template<typename T>
    class matrix : public vec<T> {
    private:
    
        size_t n_rows;
        size_t n_cols;

    public:
        // constructors
        matrix(size_t rows, size_t cols) : n_rows(rows), n_cols(cols) {
            this->N = rows * cols;
            this->data_.resize(rows * cols);
        }

        matrix(std::initializer_list<std::initializer_list<T>> list) : n_rows(list.size()) {
            size_t cols = 0;
            for (auto row : list) {
                for (auto elem : row) {
                    this->data_.push_back(elem);
                    cols++;
                }
            }
            n_cols = cols / n_rows;
            this->N = n_rows * n_cols;
        }

        matrix(matrix const& other) : n_rows(other.n_rows), n_cols(other.n_cols) {
            this->N = other.N;
            this->data_ = other.data_;
        }

        matrix() = default;

        // destructor
        ~matrix() = default;

        // element reference
        T& operator()(const size_t i, const size_t j) {
            return this->data_[i*n_cols + j];
        }
        
        const T& operator()(const size_t i, const size_t j) const {
            return this->data_[i*n_cols + j];
        }

        // reference to the whole row
        vec<T> operator[](const size_t row) const {
            vec<T> res(n_cols);
            for (size_t i = 0; i < n_cols; i++) {
                res[i] = (*this)(row, i);
            }
            return res;
        }

        // operators
        matrix& operator=(const T& value) {
            for (size_t i = 0; i < this->N; i++) {
                this->data_[i] = value;
            }
            return *this;
        }

		matrix operator*(const T &scalar) const {
            matrix res = *this;
			for (size_t i = 0; i < this->N; i++) {
				res.data_[i] *= scalar;
			}
			return res;
		}

		matrix operator/(const T &scalar) const {
            matrix res = *this;
			for (size_t i = 0; i < this->N; i++) {
				res.data_[i] /= scalar;
			}
			return res;
		}

		matrix& operator*=(const T &scalar) {
			*this = (*this) * scalar;
			return *this;
		}

		matrix& operator/=(const T &scalar) {
			*this = (*this) / scalar;
			return *this;
		}

        matrix operator+(const matrix &other) const {
            matrix result = *this;
            for (size_t i = 0; i < this->N; i++) {
                result.data_[i] = this->data_[i] + other.data_[i];
            }
            return result;
        }

        matrix operator-(const matrix &other) const {
            matrix result = *this;
            for (size_t i = 0; i < this->N; i++) {
                result.data_[i] = this->data_[i] - other.data_[i];
            }
            return result;
        }

        matrix& operator+=(const matrix &other){
            *this = (*this) + other;
            return *this;
        }

        matrix& operator-=(const matrix &other){
            *this = (*this) - other;
            return *this;
        }

        // create an indentity matrix
        matrix identity() {
            matrix res(n_rows, n_cols);
            for (size_t i = 0; i < n_rows; i++) {
                for (size_t j = 0; j < n_cols; j++) {
                    res(i, j) = (i == j) ? 1 : 0;
                }
            }
            return res;
        }

        // matrix transpose
        matrix transpose() {
            matrix res(n_cols, n_rows);
            for (size_t i = 0; i < n_rows; i++) {
                for (size_t j = 0; j < n_cols; j++) {
                    res(j, i) = (*this)(i, j);
                }
            }
            return res;
        }

        // matrix inverse
        matrix inv() {
            assert(n_rows == n_cols);
            matrix target = *this;
            matrix res = (*this).identity();
            size_t n = n_rows;           

            auto swapRows = [&](matrix& res, size_t row1, size_t row2) {
                for (size_t k = 0; k < n_cols; k++) {
                    std::swap(res(row1, k), res(row2, k));
                }
            };

            // Perform Gaussian elimination with partial pivoting
            for (size_t i = 0; i < n; ++i) {
                // Find the pivot element
                double maxElement = std::abs(target(i, i));
                int maxRow = i;
                for (size_t k = i + 1; k < n; ++k) {
                    if (std::abs(target(k, i)) > maxElement) {
                        maxElement = std::abs(target(k, i));
                        maxRow = k;
                    }
                }

                // Swap the maximum element row with the current row in both matrices
                swapRows(target, i, maxRow);
                swapRows(res   , i, maxRow);

                // Make the diagonal element 1
                double div = target(i, i);
                for (size_t j = i; j < n; ++j) {
                    target(i, j) /= div;
                }
                for (size_t j = 0; j < n; ++j) {
                    res(i, j) /= div;
                }

                // Make other elements in this column zero
                for (size_t j = 0; j < n; ++j) {
                    if (j != i) {
                        double factor = target(j, i);
                        for (size_t k = i; k < n; ++k) {
                            target(j, k) -= factor * target(i, k);
                        }
                        for (size_t k = 0; k < n; ++k) {
                            res(j, k) -= factor * res(i, k);
                        }
                    }
                }
            }   

            return res;
        }

        void fill_column(size_t col, const vec<T>& v) {
            assert(v.size() == n_rows);
            for (size_t i = 0; i < n_rows; i++) {
                (*this)(i, col) = v[i];
            }
        }

        void fill_row(size_t row, const vec<T>& v) {
            assert(v.size() == n_cols);
            for (size_t i = 0; i < n_cols; i++) {
                (*this)(row, i) = v[i];
            }
        }
        
        // Accessors
        size_t rows() const { return n_rows; }
        size_t cols() const { return n_cols; }

    }; // end of matrix class


    // matrix-matrix multiplication
    template<typename T>
    matrix<T> operator*(const matrix<T> &M1, const matrix<T> &M2) {

        assert(M1.cols() == M2.rows());
        matrix<T> result(M1.rows(), M2.cols());

        for (size_t i = 0; i < M1.rows(); i++) {
            for (size_t j = 0; j < M2.cols(); j++) {
                result(i, j) = 0;
                for (size_t k = 0; k < M1.cols(); k++) {
                    result(i, j) += M1(i, k) * M2(k, j);
                }
            }
        }
        return result;
    }

    // matrix-vector multiplication
    template<typename T>
    vec<T> operator*(const matrix<T> &M, const vec<T> &v) {
        assert(M.cols() == v.size());
        vec<T> result(M.rows());
        for (size_t i = 0; i < M.rows(); i++) {
            result[i] = 0;
            for (size_t j = 0; j < M.cols(); j++) {
                result[i] += M(i, j) * v[j];
            }
        }
        return result;
    }

    // print matrix
    template<typename T>
    std::ostream& operator<<(std::ostream& output, const matrix<T>& mat)
    {
        output << "Matrix(" << mat.rows() << ", " << mat.cols() << "):" << std::endl;
        for (size_t i = 0; i < mat.rows(); i++) {
            output << "[";
            for (size_t j = 0; j < mat.cols(); j++) {
                output << mat(i, j);
                if (j < mat.cols() - 1) output << ", ";
            }
            output << "]" << std::endl;
        }
        return output;
    }
}

