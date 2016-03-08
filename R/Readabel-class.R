#' Proxy of a binary OmicABEL output file.
#'
#' @slot pointer External pointer to a C++ object.
setClass("Readabel",
    slots = list(
        pointer = "externalptr",
        cache = "environment"))

setGeneric("names")

#' Return column names.
#'
#' @param x An object of class Readabel.
#' @export
setMethod("names", "Readabel", function(x) {
    .Call("rcpp_names", x@pointer, PACKAGE = "Readabel")
})

#' Return a new Readabel object.
#'
#' @param x An uninitialized object of class Readabel
#' @param layout_file Path to layout file
#' @param data_file Path to data file
setMethod("initialize", "Readabel", function(.Object, layout_file, data_file) {
    .Object@pointer <- .Call("rcpp_new", layout_file, data_file, PACKAGE = "Readabel")
    .Object@cache <- new.env(parent = emptyenv())
    .Object
})

#' Read a binary OmicABEL file.
#'
#' @param layout_file Path to layout file.
#' @param data_file Path to data file.
#' @export
read_omicabel <- function(layout_file, data_file) {
    new("Readabel", layout_file, data_file)
}

setGeneric("nrow")

#' Return number of rows.
#'
#' @param x An object of class Readabel
#' @export
setMethod("nrow", "Readabel", function(x) {
    .Call("rcpp_nrow", x@pointer, PACKAGE = "Readabel")
})

setGeneric("ncol")

#' Return number of columns.
#'
#' @param x An object of class Readabel
#' @export
setMethod("ncol", "Readabel", function(x) {
    length(names(x))
})

setGeneric("dim")

#' Return dimensions
#'
#' @param x An object of class Readabel
#' @export
setMethod("dim", "Readabel", function(x) {
    c(nrow(x), ncol(x))
})

setGeneric("snpNames", function(x) standardGeneric("snpNames"))

#' Return snp labels
#'
#' @param x An object of class Readabel
#' @export
setMethod("snpNames", "Readabel", function(x) {
    .Call("rcpp_snpNames", x@pointer, PACKAGE = "Readabel")
})

setGeneric("traitNames", function(x) standardGeneric("traitNames"))

#' Return trait labels
#'
#' @param x An object of class Readabel
#' @export
setMethod("traitNames", "Readabel", function(x) {
    .Call("rcpp_traitNames", x@pointer, PACKAGE = "Readabel")
})

setGeneric("[[")

#' Return a given column
#'
#' @param x An object of class Readabel
#' @param i Column name or column index
#' @export
setMethod("[[", "Readabel", function(x, i) {
    if (is.numeric(i))
        i <- names(x)[[i]]
    if (! i %in% names(x))
        stop("invalid column: ", i)
    if (is_in_cache(x, i))
        return(find_in_cache(x, i))
    column <- switch(i,
        trait = .Call("rcpp_get_trait_column", x@pointer, PACKAGE = "Readabel"),
        snp = .Call("rcpp_get_snp_column", x@pointer, PACKAGE = "Readabel"),
        .Call("rcpp_get_numeric_column", x@pointer, i, vector("double", nrow(x)), PACKAGE = "Readabel"))
    add_to_cache(x, i, column)
    column
})

setGeneric("$")

#' Return a given column
#'
#' @param x An object of class Readabel
#' @param name A column name
#' @export
setMethod("$", "Readabel", function(x, name) {
    x[[name]]
})

setGeneric("[")

#' Extract a subset of rows and columns
#'
#' @param x An object of class Readabel
#' @param i Row name or row index (logical or numeric)
#' @param j Column name or column index (logical or numeric)
#' @param drop If TRUE (default) the result will be coerced to the
#'             lowest possible dimension.  Otherwise the result will
#'             be a data frame.
#' @export
setMethod("[", "Readabel", function(x, i, j, drop = TRUE) {
    make_data_frame_from_columns <- function(columns) {
        if (is.logical(columns) || is.numeric(columns))
            columns <- names(x)[columns]
        d <- data.frame(lapply(columns, function(col) x[[col]]), stringsAsFactors = FALSE)
        names(d) <- columns
        d[,, drop]
    }
    if (missing(i) && missing(j))
        return(make_data_frame_from_columns(names(x)))
    if (missing(i) && !missing(j))
        return(make_data_frame_from_columns(j))
    if (!missing(i) && missing(j))
        return(x[][i, ])
    return(x[, j][i, ])
})

setGeneric("head")

#' Return the first few rows of a Readabel object
#'
#' @param x An object of class Readabel
#' @param n Number of rows to return
#' @export
setMethod("head", "Readabel", function(x, n = 6L) {
    x[][seq_len(n), ]
})

setGeneric("tail")

#' Return the last few rows of a Readabel object
#'
#' @param x An object of class Readabel
#' @param n Number of rows to return
#' @export
setMethod("tail", "Readabel", function(x, n = 6L) {
    x[][-seq_len(nrow(x) - n), ]
})

setGeneric("colnames")

#' Return the column names of a Readabel object
#'
#' @param x An object of class Readabel
#' @export
setMethod("colnames", "Readabel", function(x) {
    names(x)
})

setGeneric("rownames")

#' Return the row names of a Readabel object
#'
#' @param x An object of class Readabel
#' @export
setMethod("rownames", "Readabel", function(x) {
    as.character(seq_len(nrow(x)))
})

setGeneric("row.names")

#' Return the row names of a Readabel object
#'
#' @param x An object of class Readabel
#' @export
setMethod("row.names", "Readabel", function(x) {
    rownames(x)
})

setGeneric("dimnames")

#' Return set of dimnames of a Readabel object
#'
#' @param x An object of class Readabel
#' @export
setMethod("dimnames", "Readabel", function(x) {
    list(rownames(x), colnames(x))
})

setGeneric("print")

#' Print representation of a Readabel object
#'
#' @param x An object of class Readabel
#' @export
setMethod("print", "Readabel", function(x) {
    number_of_traits <- length(traitNames(x))
    number_of_snps <- length(snpNames(x))
    covariates <- sub("beta_", "", grep("^beta", names(x), ignore.case = TRUE, value = TRUE), ignore.case = TRUE)
    cat("traits: ", number_of_traits, "  ",
        "snps: ", number_of_snps, "  ",
        "covariates: ", paste(covariates, collapse = " "),
        "\n", sep = "")
})

setGeneric("show")

#' Print representation of a Readabel object
#'
#' @param object An object of class Readabel
#' @export
setMethod("show", "Readabel", function(object) {
    print(object)
})

setGeneric("as.data.frame")

#' Coerce a Readabel object into a data.frame
#'
#' @param x An object of class Readabel
#' @export
setMethod("as.data.frame", "Readabel", function(x) {
    x[]
})

setGeneric("is_in_cache", function(x, name) stop("not implemented"))

#' Is a given column in the cache of x?
#'
#' @param x An object of class Readabel
#' @param name A column name
setMethod("is_in_cache", "Readabel", function(x, name) {
    exists(name, envir = x@cache, inherits = FALSE)
})

setGeneric("add_to_cache", function(x, name, object) stop("not implemented"))

#' Add object to cache of x under name
#'
#' @param x An object of class Readabel
#' @param name A column name
#' @param object A column vector of x
setMethod("add_to_cache", "Readabel", function(x, name, object) {
    assign(name, object, envir = x@cache)
})

setGeneric("find_in_cache", function(x, name) stop("not implemented"))

#' Find name in cache of x
#'
#' @param x An object of class Readabel
#' @param name A column name
setMethod("find_in_cache", "Readabel", function(x, name) {
    get(name, envir = x@cache, inherits = FALSE)
})

setGeneric("clear_cache", function(x) stop("not implemented"))

#' Clear cache of x
#'
#' @param x An object of class Readabel
setMethod("clear_cache", "Readabel", function(x) {
    cached_objects <- ls(envir = x@cache)
    remove(list = cached_objects, envir = x@cache)
})
