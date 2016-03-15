source("setup.R")

context("[")

test_that("x[, 'snp', drop = FALSE]", {
    clear_cache(x)
    expect_equal(d[, "snp", drop = FALSE],
                 x[, "snp", drop = FALSE])
})

test_that("x[, c('snp', 'beta_snp'), drop = FALSE]", {
    clear_cache(x)
    expect_equal(d[, c("snp", "beta_snp"), drop = FALSE],
                 x[, c("snp", "beta_snp"), drop = FALSE])
})

test_that("x[, 2]", {
    clear_cache(x)
    expect_equal(d[, 2], x[, 2])
})

test_that("x[, 2:3]", {
    clear_cache(x)
    expect_equal(d[, 2:3], x[, 2:3])
})

test_that("x[]", {
    clear_cache(x)
    expect_equal(d[], x[])
})

test_that("x[1:2]", {
    clear_cache(x)
    cols <- 1:2
    expect_equal(d[cols], x[cols])
})

test_that("x[,]", {
    clear_cache(x)
    expect_equal(d[, ], x[, ])
})

test_that("x[c(2,5), ]", {
    clear_cache(x)
    rows <- c(2,5)
    expect_equal(d[rows, ], x[rows, ])
})

test_that("x[1:2, 1]", {
    clear_cache(x)
    rows <- 1:2
    expect_equal(d[rows, 1], x[rows, 1])
})

test_that("x[-1, ]", {
    clear_cache(x)
    expect_equal(d[-1,], x[-1,])
})

test_that("x[c(3,6), c(2,7)]", {
    clear_cache(x)
    rows <- c(3,6)
    cols <- c(2,7)
    expect_equal(d[rows, cols], x[rows, cols])
})

test_that("x[c(TRUE, FALSE), ]", {
    clear_cache(x)
    expect_equal(d[c(TRUE, FALSE), ], x[c(TRUE, FALSE), ])
})

test_that("x[, c(TRUE, FALSE)]", {
    clear_cache(x)
    expect_equal(d[, c(TRUE, FALSE)], x[, c(TRUE, FALSE)])
})

test_that("x[, character()]", {
    clear_cache(x)
    skip("needed for compatibility with [.data.frame but not crucial")
    expect_equal(d[, character()], x[, character()])
})

test_that("x[integer(), ]", {
    clear_cache(x)
    expect_equal(d[integer(), ], x[integer(), ])
})

test_that("x[, c('beta_intercept', 'trait')]", {
    clear_cache(x)
    cols <- c("beta_intercept", "trait")
    expect_equal(d[, cols], x[, cols])
})

test_that("x[2:1, ]", {
    clear_cache(x)
    rows <- 2:1
    expect_equal(d[rows, ], x[rows, ])
})
