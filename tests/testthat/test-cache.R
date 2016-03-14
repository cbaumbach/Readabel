source("setup.R")

context("cache")

test_that("caching works with [[", {
    clear_cache(x)
    column <- "snp"
    expect_false(is_in_cache(x, column))
    x[[column]]
    expect_true(is_in_cache(x, column))
})
