cc_library(
    name = "poker",
    srcs = [
        "card.cpp",
        "card.h",
        "deck.cpp",
        "deck.h",
        "determine_hand.cpp",
        "determine_hand.h",
        "hand.cpp",
        "hand.h",
        "polymorphic_iterator.h",
        "span.h",
    ],
    deps = [
        "@Regex//:util",
    ],
)

cc_binary(
    name = "Pokerrechner",
    srcs = [
        "Pokerrechner.cpp",
    ],
    deps = [":poker"],
)

cc_binary(
    name = "fuzz-klee",
    srcs = [
        "klee_main.cpp",
    ],
    linkopts = [
        "-lkleeRuntest",
    ],
    deps = [":poker"],
)

cc_binary(
    name = "fuzz-llvm",
    srcs = [
        "libfuzzer_entrypoint.cpp",
    ],
    copts = ["-fsanitize=fuzzer"],
    linkopts = ["-fsanitize=fuzzer"],
    deps = [
        ":poker",
    ],
)

cc_test(
    name = "determine_hand_test",
    srcs = [
        "test_determine_hand.cpp",
    ],
    deps = [
        ":poker",
        "@com_google_googletest//:gtest_main",
    ],
)

# clang++ -std=c++2a -g -stdlib=libc++  Pokerrechner.cpp -lkleeRuntest
# clang++ -std=c++2a -g -fprofile-arcs -ftest-coverage -O2  Pokerrechner.cpp -fsanitize=fuzzer
# clang++ -std=c++2a -emit-llvm -c -g -stdlib=libc++ -O1 -Xclang -disable-llvm-passes Pokerrechner.cpp -D__NO_STRING_INLINES -D_FORTIFY_SOURCE=0 -U__OPTIMIZE__

load("@bazel_compilation_database//:aspects.bzl", "compilation_database")

compilation_database(
    name = "compdb",
    # ideally should be the same as `bazel info execution_root`.
    exec_root = "/home/racko/.cache/bazel/_bazel_racko/cdbb3f90fef29ce116ee525ad2423013/execroot/__main__",
    targets = [
        "//:fuzz-llvm",
        #"//:fuzz-klee",
        "//:Pokerrechner",
        #"//:determine_hand_test",
    ],
)
