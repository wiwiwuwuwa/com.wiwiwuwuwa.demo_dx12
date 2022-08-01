#pragma once
#include <pch.h>

namespace aiva::utils
{
    template <typename TContainer>
    struct TSignalAggregator final
    {
        using result_type = TContainer;

        template <typename Iter>
        TContainer operator()(Iter const first, Iter const last) const
        {
            auto values = TContainer{};
            std::copy(first, last, std::back_inserter(values));

            return values;
        }
    };
}
