#pragma once

#include <iostream>

namespace custom{
    template <typename T>
    class array{
        private:
            T *array_values;
        public:
            const int length;

            explicit array <T> (int size) :
                    array_values(new T[size]), length(size){}

            array <T> (array &original) :
                    array_values(new T[original.length]), length(original.length){
                memcpy(array_values, original.array_values, (original.length * sizeof *array_values));
            }

            T& operator[](const unsigned int& index) {
                if(index < 0 || index >= length){
                    std::cerr << "array out of bounds" << std::endl;
                }
                else{
                    return array_values[index];
                }
            }

            friend std::ostream& operator<<(std::ostream& stream, const array<T> &my_array){
                for(int x1 = 0; x1 < my_array.length; ++x1){
                    stream << (int) my_array.array_values[x1] << " ";
                }
                return stream;
            }

            friend bool operator==(const array<T> &my_array1, const array<T> &my_array2){
                if(my_array1.length != my_array2.length){
                    return false;
                }
                else{
                    for (int x1 = 0; x1 < my_array1.length; ++x1) {
                        if(my_array1.array_values[x1] != my_array2.array_values[x1]){
                            return false;
                        }
                    }
                    return true;
                }
            }

            ~array(){
                std::cout << "memory freed" << std::endl;
                delete[] array_values;
            }
    };
}

