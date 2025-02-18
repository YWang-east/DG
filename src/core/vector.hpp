#pragma once

#include <vector>
#include <cstddef>
#include <iostream>

namespace DG::core{
    /** 
     * @brief data type: vector
     * 
     * wraps around a std::vector to provide vector operations in linear algebra
     */
    template<typename T>
    class vec {

    protected:

        size_t N;               // size
        std::vector<T> data_;   // elements

    public:
        // constructors
        vec() = default;
        vec(std::initializer_list<T> list) : N(list.size()), data_(list) {}
        vec(size_t size) : N(size) {data_.resize(size);}
        vec(const vec& other) : N(other.N), data_(other.data_) {}

        // destructor
        virtual ~vec() = default;

        // size
        size_t size() const {
            return N;
        }

        void resize(const size_t len) {
            N = len;
            data_.resize(len);
        }

        // element reference
        T& operator[](const size_t i) {
            return data_[i];
        }

        const T& operator[](const size_t i) const {
            return data_[i];
        }

        // operators
        vec& operator=(const T& value) {
            for (size_t i = 0; i < N; i++) {
                data_[i] = value;
            }
            return *this;
        }

		vec operator*(const T &scalar) const {
            vec res = *this;
			for (size_t i = 0; i < N; i++) {
				res[i] *= scalar;
			}
			return res;
		}

		vec operator/(const T &scalar) const {
            vec res = *this;
			for (size_t i = 0; i < N; i++) {
				res[i] /= scalar;
			}
			return res;
		}

		vec& operator*=(const T &scalar) {
			*this = (*this * scalar);
			return *this;
		}

		vec& operator/=(const T &scalar) {
			*this = (*this) / scalar;
			return *this;
		}

        vec operator+(const vec &other) const {
            vec result = *this;
            for (size_t i = 0; i < N; i++) {
                result[i] = data_[i] + other.data_[i];
            }
            return result;
        }

        vec operator-(const vec &other) const {
            vec result = *this;
            for (size_t i = 0; i < N; i++) {
                result[i] = data_[i] - other.data_[i];
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

        T norm() const {
            T result = 0;
            for (size_t i = 0; i < N; i++) {
                result += data_[i] * data_[i];
            }
            return std::sqrt(result);
        }

        T Linf_norm() const {
            vec result = *this;
            for (size_t i = 0; i < N; i++) {
                result[i] = std::abs(data_[i]);
            }

            return result.max();
        }
    };

    // print vector
    template<typename T>
    std::ostream& operator<<(std::ostream& output, const vec<T>& v)
    {
        output << "Vector(" << v.size() << "):" << std::endl;
        output << "[";
        for (size_t i = 0; i < v.size(); i++) {
            output << v[i];
            if (i < v.size() - 1) output << ", ";
        }
        output << "]" << std::endl;
        return output;
    }
}
