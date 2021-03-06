% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/bzinbReg.R
\docType{data}
\name{bzinbReg}
\alias{bzinbReg}
\alias{bzinbReg.default}
\alias{bzinbReg.formula}
\alias{print.bzinbReg}
\alias{expt.names}
\title{The bivariate zero-inflated negative binomial regression.}
\format{
An object of class \code{character} of length 12.
}
\usage{
bzinbReg(x, ...)

expt.names

\method{bzinbReg}{formula}(
  mu.formula,
  nu.formula = ~1,
  data,
  zero.inflation = c("full", "co-ZI", "ZINB-NB", "NB-ZINB", "BNB"),
  tol = 1e-08,
  maxiter = 50000,
  showFlag = FALSE,
  vcov = FALSE,
  initial = NULL
)

\method{print}{bzinbReg}(x, digits = max(3L, getOption("digits") - 3L), ...)
}
\arguments{
\item{tol}{tolerance for judging convergence. \code{tol = 1e-8} by default.}

\item{maxiter}{maximum number of iterations allowed. \code{tol = 50000} by default.}

\item{showFlag}{if \code{TRUE}, the updated parameter estimates for each iteration 
are printed out. If a positive integer, the updated parameter estimates for 
iterations greater than \code{showFlag} are printed out.}

\item{vcov}{if \code{TRUE}, the variance-covariance matrix and information matrix 
are returned.}

\item{initial}{starting value of param for EM algorithm, a vector of nine values.}

\item{y1, y2}{a pair of bzinb random vectors. nonnegative integer vectors. 
If not integers, they will be rounded to the nearest integers.}

\item{n}{number of observations.}
}
\value{
\itemize{
   \item the regression coefficients by the MLE of the BZINB regression model.
     \itemize{
       \item \code{coefficients} estimate and standard error of the BZINB parameters
       \item \code{lik} log-likelihood of the maximum likelihood estimate
       \item \code{iter} total number of iterations
       \item \code{info} information matrix. Provided when \code{vcov} is \code{TRUE}.
       \item \code{vcov} variance-covariance matrix. Provided when \code{vcov} is \code{TRUE}.
     }
   \item \code{lik.bzinb} gives the log-likelihood of a set of parameters for a BZINB pair.

 }
}
\description{
the bivariate zero-inflated negative regression.
}
\details{

}
\examples{
library(devtools)
document()
load_all()
set.seed(1)
dat <- rBzinbData()
bzinbReg(cbind(y1, y2) ~ ., ~ X1, data = dat, maxiter = 10)
print(bzinbReg(cbind(y1, y2) ~ ., ~ X1, data = dat, maxiter = 10))
bzinbReg(cbind(y1, y2) ~ ., ~ X1, data = dat, zero.inflation = "co-ZI", maxiter = 10)
bzinbReg(cbind(y1, y2) ~ ., ~ X1, data = dat, zero.inflation = "ZINB-NB", maxiter = 10)
bzinb(y1 = data1[,1], y2 = data1[,2], showFlag = FALSE)

}
\references{
Cho, H., Preisser, J., Liu, C., and Wu, D. (In preparation), "A bivariate 
 zero-inflated negative binomial model for identifying underlying dependence"

 Dempster, A. P., Laird, N. M., & Rubin, D. B. (1977). Maximum likelihood from 
 incomplete data via the EM algorithm. Journal of the Royal Statistical Society: 
 Series B (Methodological), 39(1), 1-22.
}
\author{
Hunyong Cho, Chuwen Liu, Jinyoung Park, and Di Wu
}
\keyword{datasets}
