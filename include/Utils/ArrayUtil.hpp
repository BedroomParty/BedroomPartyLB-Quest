#pragma once

#include "beatsaber-hook/shared/utils/typedefs-array.hpp"

namespace BedroomPartyLB::ArrayUtil{
    // yoinked from questui
    template <class T, class Predicate>
    inline ArrayW<T> Where(ArrayW<T> array, Predicate pred)
    {
        List<T>* newArray = List<T>::New_ctor();
        for (T item : array) {
            if(pred(item))
                newArray->Add(item);
        }
        return newArray->ToArray();
    }
}