cc_library(
    name = "lib",
    srcs = ["calc.cc"],
    hdrs = ["calc.h"],
    visibility = ["//test:__pkg__"],
)

cc_library(
    name = "ast",
    srcs = ["ast.c",],
    hdrs = ["ast.h",],
)

cc_test(
    name = "test",
    srcs = ["test-ast.cc",],
    deps = ["@com_google_googletest//:gtest_main", "//calc:ast"],
)
