#' Proxy of a binary OmicABEL output file.
#'
#' @slot pointer External pointer to a C++ object.
setClass("Readabel",
    slots = list(pointer = "externalptr"))

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
    if (i == "trait")
        .Call("rcpp_get_trait_column", x@pointer, PACKAGE = "Readabel")
    else if (i == "snp")
        .Call("rcpp_get_snp_column", x@pointer, PACKAGE = "Readabel")
    else
        .Call("rcpp_get_numeric_column", x@pointer, i, PACKAGE = "Readabel")
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
        if (is.numeric(columns))
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
