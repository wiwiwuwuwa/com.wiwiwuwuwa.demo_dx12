#pragma once

namespace aiva
{
    struct i_support_unordered_access_t
    {
        // ------------------------------------------------

    public:
        virtual auto get_support_unordered_access() const -> bool = 0;

        virtual void set_support_unordered_access(bool const support_unordered_access) = 0;

        // ------------------------------------------------
    };
} // namespace aiva
