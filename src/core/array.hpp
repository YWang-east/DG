#pragma once

#include <cstddef>
#include <iostream>
#include "vector.hpp"

namespace DG::core{
    /** 
     * @brief data type: fixed size array
     * 
     * wraps around a std::vector to provide vector operations in linear algebra
     */
    template<typename T, const size_t N>
    class arr {
    private:

        std::array<T, N> data_;   // elements

    public:
        // assignment with a list
        template<typename... Ss>
		constexpr arr(const Ss &...s) {
			data_ = {s...};
		}

        // assignment with a vector
        arr(const vec<T> &v) {
            assert(v.size() == N && "Size mismatch.");
            for (size_t i = 0; i < N; i++) {
                data_[i] = v[i];
            }
        }

        // element reference
        T& operator[](const size_t i) {
            return data_[i];
        }

        const T& operator[](const size_t i) const {
            return data_[i];
        }

        // operators
        arr& operator=(const T& value) {
            for (size_t i = 0; i < N; i++) {
                data_[i] = value;
            }
            return *this;
        }

        template<typename T1>
		arr operator*(const T1 &scalar) const {
            arr res;
			for (size_t i = 0; i < N; i++) {
				res[i] = data_[i] * scalar;
			}
			return res;
		}

        template<typename T1>
		arr operator/(const T1 &scalar) const {
            arr res;
			for (size_t i = 0; i < N; i++) {
				res[i] = data_[i] / scalar;
			}
			return res;
		}

		arr& operator*=(const T &scalar) {
			*this = (*this * scalar);
			return *this;
		}

		arr& operator/=(const T &scalar) {
			*this = (*this) / scalar;
			return *this;
		}

        arr operator+(const arr &other) const {
            arr result = *this;
            for (size_t i = 0; i < N; i++) {
                result[i] = data_[i] + other.data_[i];
            }
            return result;
        }

        arr operator-(const arr &other) const {
            arr result = *this;
            for (size_t i = 0; i < N; i++) {
                result[i] = data_[i] - other.data_[i];
            }
            return result;
        }

        arr operator-() const {
			arr result;
			for (size_t i = 0; i < N; i++) {
				result[i] = -data_[i];
			}
			return result;
		}

        T max() const {
            T result = data_[0];
            for (size_t i = 1; i < N; i++) {
                if (data_[i] > result) result = data_[i];
            }
            return result;
        }

        T min() const {
            T result = data_[0];
            for (size_t i = 1; i < N; i++) {
                if (data_[i] < result) result = data_[i];
            }
            return result;
        }

        T norm() const {
            T result = 0;
            for (size_t i = 0; i < N; i++) {
                result += data_[i] * data_[i];
            }
            return std::sqrt(result);
        }

        void fill(T value) {
            for (size_t i = 0; i < N; i++) {
                data_[i] = value;
            }   
        }
    };

    // print vector
    template<typename T, const size_t N>
    std::ostream& operator<<(std::ostream& output, const arr<T, N>& v)
    {
        output << "[";
        for (size_t i = 0; i < N; i++) {
            output << v[i];
            if (i < N - 1) output << ", ";
        }
        output << "]" << std::endl;
        return output;
    }
}
