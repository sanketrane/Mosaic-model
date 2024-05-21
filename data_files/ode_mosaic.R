library(deSolve)
library(tidyverse)


influx_size <- function(Time){
  basl = 9; theta = 250; n = 2; X = 32; q = 4;
  return(
    exp(basl) + (theta * Time^n) * (1 - (Time^q / (X^q + Time^q)))
         )
}

ts_vec <- exp(seq(log(5), log(300), length.out = 100))
infl_vec <- sapply(ts_vec, influx_size)

ggplot() +
  geom_point(aes(x = ts_vec, y = infl_vec), col = 4) +
  scale_x_log10()
  

sys_eq <- function(Time, State, Pars) {
  with(as.list(c(State, Pars)), {
    ## naive displaceable
    dx1 <- psi * influx_size(Time) + rho_dis * x1 - (delta_dis + alpha) * x1
    
    ## naive Incumbent
    dx2 <- (rho_inc + alpha) * x2 - (delta_inc + alpha) * x2
    
    ## memory fast
    dx3 <- alpha * (x1 + x2) + kappa_f * x3 - (lambda_f + mu) * x3
    
    ## memory slow
    dx4 <- mu * x3 + kappa_s * x4 - lambda_s * x4
    
    return(list(c(dx1, dx2, dx3, dx4)))
  })
}

State <- c(x1 = 50000, x2=50000, x3= 9000, x4=1000)
Pars <- c(psi = 0.4, rho_dis = 0.017, delta_dis = 0.035, alpha = 0.01,
          rho_inc = 0.061, delta_inc = 0.061,
          kappa_f = 0.15, kappa_s = 0.023, mu = 0.23,
          lambda_f = 0.1, lambda_s = 0.022)

sys_sol <- data.frame(ode(State, ts_vec, sys_eq, Pars)) %>%
  rename(Timeseries = time,
         naiDis = x1,
         naiInc = x2,
         memFast = x3,
         memSlow = x4
  ) %>%
  gather(-Timeseries, key = "popl", value = "cell_counts")


ggplot(sys_sol) +
  geom_line(aes(x = Timeseries, y = log(cell_counts), col = popl)) +
  #scale_y_log10() +
  facet_wrap(. ~ popl, scales = "free_y")








