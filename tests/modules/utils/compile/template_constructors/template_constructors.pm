{
    test_cases => {
        "test_non_default_constructible_pointer" => [
            "draupnir::utils::create_tuple_new<std::tuple<int*,NonDefaultCtr*>>()",
            "assertion failed: Provided tuple contains one or more pointers to objects which can not be dafult constructed (using new T)",
            "static_assert(is_tuple_like_pointees_default_constructible_v<Tuple>"
        ],
    },
}
