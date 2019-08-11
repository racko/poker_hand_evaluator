local_repository(name = "Regex", path = "/home/racko/btsync/git/Regex")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

#http_archive(
#    name = "bazel-compilation-database",
#    urls = ["https://github.com/grailbio/bazel-compilation-database/archive/0.3.0.tar.gz"],
#    strip_prefix = "bazel-compilation-database-0.3.0",
#    sha256 = "2f70d39fab288a3fb2dbeeeb023a1695deeaf9fd22ad1755cd9d42cfeead96c9",
#)
local_repository(name = "bazel_compilation_database", path = "/home/racko/tmp/bazel-compilation-database-0.3.0")

http_archive(
     name = "com_google_googletest",
     urls = ["https://github.com/google/googletest/archive/b6cd405286ed8635ece71c72f118e659f4ade3fb.zip"],  # 2019-01-07
     strip_prefix = "googletest-b6cd405286ed8635ece71c72f118e659f4ade3fb",
     sha256 = "ff7a82736e158c077e76188232eac77913a15dac0b22508c390ab3f88e6d6d86",
)

# Google benchmark.
http_archive(
    name = "com_github_google_benchmark",
    urls = ["https://github.com/google/benchmark/archive/16703ff83c1ae6d53e5155df3bb3ab0bc96083be.zip"],
    strip_prefix = "benchmark-16703ff83c1ae6d53e5155df3bb3ab0bc96083be",
    sha256 = "59f918c8ccd4d74b6ac43484467b500f1d64b40cc1010daa055375b322a43ba3",
)
