### data wrangling for T regulatory cells from Busulfan chimera experiments

## claning environment and cache
rm(list = ls()); gc()

#Loading required libraries
library(tidyverse)
library(deSolve)

myTheme <-  theme(axis.text = element_text(size = 14),
                  axis.title =  element_text(size = 14, face = "bold"),
                  plot.title = element_text(size = 14, face = "bold",  hjust = 0.5),
                  legend.text = element_text(size = 12),
                  legend.title = element_text(size = 12, face = "bold"),
                  strip.text = element_text(size = 14))


# setting ggplot theme for rest fo the plots
theme_set(theme_bw())

fancy_scientific <- function(l) {
  # turn in to character string in scientific notation
  l <- format(l, scientific = TRUE)
  # quote the part before the exponent to keep all the digits
  l <- gsub("^(.*)e", "'\\1'e", l)
  # remove + after exponent, if exists. E.g.: (e^+2 -> e^2)
  l <- gsub("e\\+","e",l)  
  # turn the 'e' into plotmath format
  l <- gsub("e", "%*%10^", l)
  # convert 1x10^ or 1.000x10^ -> 10^
  l <- gsub("\\'1[\\.0]*\\'\\%\\*\\%", "", l)
  # return this as an expression
  parse(text=l)
}

log10minorbreaks=as.numeric(1:10 %o% 10^(4:8))


#### Putative precursors for memory Tregs: naive Tregs, conventional naive and total cd4 (naive + memory) 
### memory T cells
ontogeny_df <- readxl::read_excel(path = "data/Treg_ontogeny.xlsx", sheet = 4) %>%
  select(mouse.ID, age.at.S1K, contains("Treg"), contains("CD4"))

ont_Treg_df <- ontogeny_df %>%
  select(mouse.ID, age.at.S1K, TOTnaiTreg, TOTmemTreg) %>%
  rename(naive = TOTnaiTreg, memory = TOTmemTreg) %>%
  gather(c(naive, memory), key='popln', value = 'total_counts')



ggplot() +
  geom_point(data = ont_Treg_df, aes(x=age.at.S1K, y=total_counts), col=2)+
  facet_wrap(.~popln)+
  scale_y_log10() + scale_x_log10()


## influx
## phenomenological functionx
counts_spline <- function(Time, pars){
  basl = pars[1]; q = pars[2]; n= pars[3]; X=pars[4];
  #time_b = (Time - 296) *(-1)
  return(exp(basl) + (theta * time_b^n) * (1 - ((time_b^q)/((X^q) + (time_b^q)))))
  #val = exp(basl) * (1 + Time * exp(-Time/q)^n) +  exp(X) 
  #return(val)
}

counts_transf <- function(rev_t, tb, pars){
  Time = tb - rev_t 
  counts_spline(Time, pars)
}


## naive T cells
## LL function for counts
# count_obj <- function(params, df_pop) {
# 
#   ## SSR
#   df_merge <- rev_ont %>%
#     gather(-c(mouse.ID, rev_time, age.at.S1K), key="popln", value="total_counts") %>%
#     filter(popln == df_pop) %>%
#     mutate(count_fit = counts_spline(age.at.S1K,
#                                     params),
#            log_sr = (log(total_counts) - log(count_fit)) ^ 2) %>%
#     summarize(log_ssr = sum(log_sr)) %>%
#     unlist()
# }
# 
# # initial guess for LL calculation
# param <- list(basl=9, q=210, n=3, X=4)
# 
# ## model fit
# counts_fit <- optim(param, count_obj, df_pop = "TH.nai.Treg", control = list(trace = TRUE, maxit = 1000))
# counts_pars_est <- counts_fit$par
# 
# #parss <- c(basl=13.5, q=210, n=2.6, X=13.7)
# counts_fit_spline <- data.frame(Timeseq, "y_sp" = counts_spline(Timeseq, param))
# #counts_fit_spline2 <- data.frame(Timeseq, "y_sp" = counts_spline(Timeseq, parss))


## Time range of predictionss
ont_ThyTreg_df <- ontogeny_df %>%
  select(mouse.ID, age.at.S1K, TH.naiTreg) %>%
  rename(naive = TH.naiTreg) %>%
  gather(c(naive), key='popln', value = 'total_counts')



counts_spline <- function(Time, pars){
  basl = pars[1]; theta=pars[2]; n= pars[3]; X= pars[4]; q=pars[5];
  return(exp(basl) + (theta * Time^n) * (1 - ((Time^q)/((X^q) + (Time^q)))))
}
Timeseq <- seq(0, 400, length.out=100)
thy_counts_pars <- c(9.2, 90, 3, 32, 3.8)
thycounts_fit_m <- data.frame(Timeseq, "y_sp" = counts_spline(Timeseq, thy_counts_pars))


ggplot() + 
  geom_point(data = ont_ThyTreg_df, aes(x=age.at.S1K, y=total_counts), col=2)+
  #geom_point(data = naiTreg_buchi_df, aes(x=age.at.S1K, y=total_counts), col=7) +
  geom_line(data = thycounts_fit_m, aes(x = Timeseq , y = y_sp), col=2, size =1) + 
  #geom_line(data = thycounts_fit_m, aes(x = timeseq , y = y_sp), col=4, size =1) + 
  scale_y_log10(limits=c(1e3, 1e6)) + xlim(5, 450)+
  labs(title = 'Counts of FoxP3+ SP4 cells',  y=NULL,  x = 'Host age (days)') 

thy_counts_nlm <- nls(log(total_FoxP3_pos_SP4) ~ log(counts_spline(age.at.S1K, b0, nu)),
                      data =  denovo_df,
                      start = list(b0=5, nu=200))
thy_counts_pars <- coef(thy_counts_nlm)

thycounts_fit <- data.frame(timeseq, "y_sp" = counts_spline(timeseq, thy_counts_pars[1], thy_counts_pars[2]))
thycounts_fit_m <- data.frame(timeseq, "y_sp" = counts_spline(timeseq, 4.6, 160))

ggplot() + 
  #geom_point(data=source_join, aes(x=age.at.S1K, y=total_FoxP3_pos_SP4), col=4, size =2) +
  geom_point(data=denovo_df, aes(x=age.at.S1K, y=total_FoxP3_pos_SP4), col=2, size =2) +
  geom_line(data = thycounts_fit_m, aes(x = timeseq , y = y_sp), col=2, size =1) + 
  #geom_line(data = thycounts_fit_m, aes(x = timeseq , y = y_sp), col=4, size =1) + 
  scale_y_log10(limits=c(3e2, 3e5)) + xlim(40, 450)+
  labs(title = 'Counts of FoxP3+ SP4 cells',  y=NULL,  x = 'Host age (days)') 
naiTreg_ont_df <- ont_Treg_df %>% filter(popln == "naive")
naiTreg_buchi_df <- Buchi_counts_data %>% filter(popln == "naive")
ggplot() +
  #geom_point(data= rev_ont, aes(x=age.at.S1K, y= Tot.CD4.nai), col=4, size =2) +
  geom_point(data = naiTreg_ont_df, aes(x=age.at.S1K, y=total_counts), col=2)+
  geom_point(data = naiTreg_buchi_df, aes(x=age.at.S1K, y=total_counts), col=7) +
  geom_line(data = counts_fit_spline, aes(x = Timeseq, y = y_sp), col=4) +
  #scale_x_log10(limits= c(3, 350), breaks = c(3,10,30,100, 300)) +
  #geom_line(data = counts_fit_spline2, aes(x = Timeseq, y = y_sp), col=2) +
  labs(y =NULL, x = 'Host age (days)', title =  'Total counts in CD4 naive T cells')+
  scale_y_continuous(limits = c(1e4, 1e7), trans="log10",minor_breaks = log10minorbreaks, labels =fancy_scientific) +
  myTheme + guides(col="none")


# prediction of cell counts 
## Time range of predictions
Timeseq <- seq(0, 296, length.out=100)
## source influx
theta_spline <- function(Time, psi){
  #pars_est = c(basl=13.5, q=210, n=2.6, X=13.7)
  pars_est = c(basl=13.2, q=49, n=0.5, X=1)
  value = psi * counts_spline(Time, pars_est)
  return(value)
}

theta_rev <- function(Time, tb, psi){
  #pars_est = c(basl=13.5, q=210, n=2.6, X=13.7)
  pars_est = c(basl=13.2, q=49, n=0.5, X=1)
  value = psi * counts_transf(Time, tb, pars_est)
  return(value)
}


ode_Linear <- function (Time, y, parms) {
  with(as.list(c(y, parms)),{
    ## fast
    dy1 = theta_spline(Time, psi) - (lambda_D + mu) * y1
    ## slow
    dy2 = mu * y1 - (lambda_I) * y2
    
    list(c(dy1, dy2))
  })
}

ode_Branched <- function (Time, y, parms) {
  with(as.list(c(y, parms)),{
    ## fast
    dy1 = mu * theta_spline(Time, psi) - lambda_D * y1
    ## slow
    dy2 = (1- mu) * theta_spline(Time, psi) - lambda_I * y2
    
    list(c(dy1, dy2))
  })
}

ode_Incumbent <- function (Time, y, parms) {
  with(as.list(c(y, parms)),{
    ## fast
    y1 = theta_spline(Time, psi) - lambda_D * y1
    ## slow
    y2 = 0
  })
}



# #init_conds <- Treg_memory_Nfd$total_counts[4:8]
init_extr <- c(unlist(exp(params_mat[1, 7:10])), 0, 0, 0, 0)
ts_init <- seq(66, 70, length.out=2)
tb_init <- rep(45, length(ts_init))
param_stan <- c("psi" = params_mat$psi[100],
                "rho_d" = params_mat$rho_D[100],
                "delta_d" = params_mat$rho_D[100]  + params_mat$lambda_D[100],
                "rho_I" = params_mat$rho_I[100],
                "delta_dI" = params_mat$rho_I[100]  + params_mat$lambda_I[100],
                "mu" = params_mat$mu[100])

sol_stan <- solve_ode_chi(ts_init, tb_init, init_extr, param_stan)

stan_pred_df1 <- data.frame("age.at.S1K" = ts_init,
                            "y_pred" = matrix(unlist(sol_stan), nrow = length(ts_init), byrow = TRUE)) %>%
  mutate(total_slow = y_pred.3 + y_pred.4, + y_pred.7 + y_pred.8,
         total_fast = y_pred.1 + y_pred.2 + y_pred.5 + y_pred.6,
         fast_frac = total_fast/(total_fast+total_slow)) %>%
  select(age.at.S1K, total_fast, total_slow, fast_frac)




## preds
# first par set
## Initiating simulation
# first par set
param_init <- c("psi" = params_mat$psi[1], 
                "lambda_D" = 0.1, # params_mat$lambda_D[1], #9.5e-3
                "lambda_I" = 0.003,# params_mat$lambda_I[1], #5.6e-4
                "mu" = params_mat$mu[1])
# init conds
count_d5 <- ontogeny_df %>%
  filter(age.at.S1K == 5) %>%
  summarise(m_0 = mean(TOTmemTreg)) %>% unlist()
init_tb <- c(stan_pred_df1$total_fast[2], stan_pred_df1$total_slow[2])
#init_tb_branched <- c(count_d5 * mu_avg, count_d5 * (1-mu_avg))
init_tb_linear <- c(count_d5, 0)
names(init_tb_linear) <- c("y1", "y2")

ts_pred <- seq(5, 296, length.out=100)
## Preds for the first sampled par set
sol_pred_m <- data.frame(ode(y=init_tb_linear, times=ts_pred, func = ode_Linear, parms = param_init))%>%
  mutate(total_counts = (y1 + y2)) %>%
  select(total_counts) %>%
  bind_cols("age.at.S1K" = ts_pred)

ggplot() +
  geom_point(data = ontogeny_df, aes(x=age.at.S1K, y=TOTmemTreg), col=2)+
  geom_point(data = Buchi_counts_data, aes(x=age.at.S1K, y=total_counts), col=7) +
  #geom_line(data = sol_pred_m, aes(x=age.at.S1K, y=total_counts), col=2) +
  #geom_ribbon(data = sol_pred_bounds, aes(x=age.at.S1K, ymin = lb, ymax=ub), fill=2, alpha=0.1)+
  labs(y =NULL, x = 'Host age (days)', title =  'Total counts of memory Tregs')+
  scale_y_continuous(limits=c(1e4, 1.2e7),
    trans="log10",minor_breaks = log10minorbreaks, labels =fancy_scientific) +
  #facet_wrap(.~popln) + 
  #scale_x_log10()+
  #scale_y_log10() +
  myTheme

param_init <- c("psi" = params_mat$psi[1], 
                "lambda_D" = params_mat$lambda_D[1], #9.5e-3
                "lambda_I" = params_mat$lambda_I[1], #5.6e-4
                "mu" = params_mat$mu[1])
sol_pred <- data.frame(ode(y=init_tb_linear, times=ts_pred, func = ode_Linear, parms = param_init))%>%
  mutate(total_counts = (y1 + y2)) %>%
  select(total_counts) 
  
## iterating across param posterior
system.time(
  for (i in 2:nrow(params_mat)){
    #sample from par matrix
    param_sample <- c("psi" = params_mat$psi[i], "lambda_D" = params_mat$lambda_D[i],
                      "lambda_I" = params_mat$lambda_I[i], "mu" = params_mat$mu[i])
    
    
    #init_tb_linear <- c(rnorm(1, mean=count_d5, sd= sd(ontogeny_df$TOTmemTreg)), 0)
    #names(init_tb_linear) <- c("y1", "y2")
    
    ## generate preds for each sampled par set
    sol_pred[paste0("ont_", i)] <-
      data.frame(ode(y=init_tb_linear, times=ts_pred, func = ode_Linear, parms = param_sample)) %>%
      mutate(total_counts = (y1 + y2)) %>%
      select(total_counts)
    
    ## progress of iterations
    if (!i %% 300) print(paste0("iter_", i))
  }
)
# 
# linearTC_fast <- function(Time, parms, init_cond){
#    psi_est=parms[1]; lambda_est=(parms[2] + parms[4]); 
#    y_fast=init_cond[1];
#    
#    int_g <- function(s, psi, lambda){
#      theta_spline(s, psi) * exp(lambda * s)
#    }
#     value= (y_fast * exp(lambda_est * Time)  -
#               integrate(int_g, lower = 0, upper = Time, psi=psi_est, lambda=lambda_est)$value) 
#     return(value)
# }
# 
# linearTC_slow <- function(Time, parms, init_cond){
#   psi_est=parms[4]; lambda_est=(parms[3]); 
#   y_slow=init_cond[2];
#   
#   int_g <- function(s, parms, init_cond, psi, lambda){
#     linearTC_fast(s, parms, init_cond) * psi * exp(lambda * s)
#   }
#   value= (y_slow * exp(lambda_est * Time)  -
#             integrate(int_g, lower = 0, upper = Time, parms=parms, init_cond=init_cond,
#                       psi=psi_est, lambda=lambda_est)$value) 
#   return(value)
# }
# 
# linearTC_fast(60, param_init, init_tb)
# linearTC_slow(60, param_init, init_tb)

# incumbent_func <- function(Time, parms, init_cond){
#   psi_est=parms[1]; lambda_est=parms[2]; 
#   y_fast=init_cond[1]; y_slow=init_cond[2]
#   
#   int_g <- function(s, psi, lambda){
#     theta_spline(s, psi) * exp(lambda * s)
#   }
#    value= (y_fast * exp(lambda_est * Time)  -
#              integrate(int_g, lower = 0, upper = Time, psi=psi_est, lambda=lambda_est)$value) + 
#      y_slow
#    return(value)
# }
# 
# incumbent_func(40, param_init, init_tb)
#init_tb <- c(stan_pred_df1$total_fast[2], stan_pred_df1$total_slow[2])


sol_pred_bounds <- sol_pred %>%
  bind_cols("age.at.S1K" = ts_pred) %>%
  gather(-age.at.S1K, key="iter", value="frac") %>%
  group_by(age.at.S1K) %>%
  summarize(lb = quantile(frac, probs = 0.05),
            median = quantile(frac, probs = 0.5),
            ub = quantile(frac, probs = 0.95)) 

ggplot() +
  geom_point(data = Buchi_counts_data, aes(x=age.at.S1K, y=total_counts), col=7) +
  geom_line(data = sol_pred_bounds, aes(x=age.at.S1K, y=median), col=2) +
  #geom_line(data = sol_pred_m, aes(x=age.at.S1K, y=total_counts), linetype=2,  col=2) +
  geom_point(data = ontogeny_df, aes(x=age.at.S1K, y=TOTmemTreg), col=2)+
  geom_ribbon(data = sol_pred_bounds, aes(x=age.at.S1K, ymin = lb, ymax=ub), fill=2, alpha=0.1)+
  labs(y =NULL, x = 'Host age (days)', title =  'Total counts of memory Tregs')+
  scale_y_continuous(limits=c(1e4, 1.2e7),
                     trans="log10",minor_breaks = log10minorbreaks, labels =fancy_scientific) +
  #facet_wrap(.~popln) + 
  #scale_x_log10()+
  #scale_y_log10() +
  myTheme














































