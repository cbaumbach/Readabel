source("setup.R")

context("[")

# Full dataset.
d <- data.frame(lapply(names(x), function(name) x[[name]]), stringsAsFactors = FALSE)
names(d) <- names(x)

test_that("x[, \"snp\", drop = FALSE]", {
    expect_equal(d[, "snp", drop = FALSE],
                 x[, "snp", drop = FALSE])
})

test_that("x[, c(\"snp\", \"beta_snp\"), drop = FALSE]", {
    expect_equal(d[, c("snp", "beta_snp"), drop = FALSE],
                 x[, c("snp", "beta_snp"), drop = FALSE])
})

test_that("x[, 2]", {
    expect_equal(d[, 2], x[, 2])
})

test_that("x[, 2:3]", {
    expect_equal(d[, 2:3], x[, 2:3])
})

test_that("x[]", {
    expect_equal(d[], x[])
})

test_that("x[,]", {
    expect_equal(x[], x[, ])
})

test_that("x[c(2,5), ]", {
    rows <- c(2,5)
    expect_equal(d[rows, ], x[rows, ])
})

test_that("x[c(3,6), c(2,7)]", {
    rows <- c(3,6)
    cols <- c(2,7)
    expect_equal(d[rows, cols], x[rows, cols])
})
