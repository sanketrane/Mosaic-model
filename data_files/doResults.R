## claning environment and cache
rm(list = ls())
gc()

# Loading required libraries
library(tidyverse)
library(plotly)
library(reshape2)
library(alakazam)


my_theme <- theme(
  axis.text = element_text(size = 14),
  axis.title = element_text(size = 14, face = "bold"),
  plot.title = element_text(size = 14, face = "bold", hjust = 0.5),
  legend.text = element_text(size = 12),
  legend.title = element_text(size = 12, face = "bold"),
  strip.text = element_text(size = 14)
)


# setting ggplot theme for rest fo the plots
theme_set(theme_bw())

fancy_scientific <- function(l) {
  # turn in to character string in scientific notation
  l <- format(l, scientific = TRUE)
  # quote the part before the exponent to keep all the digits
  l <- gsub("^(.*)e", "'\\1'e", l)
  # remove + after exponent, if exists. E.g.: (e^+2 -> e^2)
  l <- gsub("e\\+", "e", l)
  # turn the 'e' into plotmath format
  l <- gsub("e", "%*%10^", l)
  # convert 1x10^ or 1.000x10^ -> 10^
  l <- gsub("\\'1[\\.0]*\\'\\%\\*\\%", "", l)
  # return this as an expression
  parse(text = l)
}

log10minorbreaks <- as.numeric(1:10 %o% 10^(1:8))

#
# thyvec1 <- sapply(timeseq, thyfunc)
# thyfunc <- function(time_b){
#   basl = 9; theta = 400; n=2; X=24; q=3
#   #time_b = (Time - 296) *(-1)
#   return(exp(basl) + (theta * time_b^n) * (1 - ((time_b^q)/((X^q) + (time_b^q)))))
# }
#
# timeseq <- seq(5, 450)
#
# thy_results <- read.csv(file = "data_files/thyResults.csv")
#
# thyvec <- sapply(timeseq, thyfunc)
#
# ggplot()+
#   geom_line(data = thy_results, aes(x=Time, y=Counts), col=4, size=2)+
#   geom_line(aes(x=timeseq, y=thyvec1), col=2)+
#   geom_line(aes(x=timeseq, y=thyvec))
#

## import data
count_results <- read.csv(file = "output_files/Results.csv") %>%
  group_by(Time) %>%
  summarise(
    "nai_disp" = sum(Total_nai_dis),
    "nai_inc" = sum(Total_nai_inc),
    "mem_fast" = sum(Total_mem_fast),
    "mem_slow" = sum(Total_mem_slow),
  ) %>%
  gather(-Time, key = "popln", value = "counts")


count_results2 <- read.csv(file = "output_files/Results.csv") %>%
  group_by(Time) %>%
  summarise(
    "nai_disp" = sum(Total_nai_dis),
    "nai_inc" = sum(Total_nai_inc),
    "mem_fast" = sum(Total_mem_fast),
    "mem_slow" = sum(Total_mem_slow),
  ) %>%
  gather(-Time, key = "popln", value = "counts")


ggplot() +
  geom_line(data = count_results, aes(x = Time, y = counts, col = popln), linetype = 2) +
  geom_line(data = count_results2, aes(x = Time, y = counts, col = popln)) +
  scale_y_log10() +
  facet_wrap(. ~ popln, scales = "free_y")


ki_results <- read.csv(file = "output_files/KiResults.csv") %>%
  group_by(Time) %>%
  summarise(
    "nai_disp" = sum(Kifrac_nai_dis),
    "nai_inc" = sum(Kifrac_nai_inc),
    "mem_fast" = sum(Kifrac_mem_fast),
    "mem_slow" = sum(Kifrac_mem_slow),
  ) %>%
  gather(-Time, key = "popln", value = "kifrac")


ggplot(ki_results) +
  geom_line(aes(x = Time, y = kifrac, col = popln)) +
  scale_y_log10()

nai_disfreq <- read.csv(file = "output_files/nai_disFreq.csv")
nai_incfreq <- read.csv(file = "output_files/nai_incFreq.csv")
mem_fastfreq <- read.csv(file = "output_files/mem_fastFreq.csv")
mem_slowfreq <- read.csv(file = "output_files/mem_slowFreq.csv")

naidis <- nai_disfreq %>%
  mutate(sample_id = ifelse(Time == 90, "d90",
    ifelse(Time == 180, "d180",
      ifelse(Time == 360, "d360", "dUU")
    )
  )) %>%
  filter(sample_id != c("dUU")) %>%
  rename(
    age_id = Agebin,
    clone_id = Clone.ID,
    seq_count = Frequency
  ) %>%
  mutate(age_in_days = ifelse(sample_id == "d90", (age_id) * 0.1,
    ifelse(sample_id == "d180", (age_id) * 0.1,
      (age_id) * 0.1
    )
  ))

naiinc <- nai_incfreq %>%
  mutate(sample_id = ifelse(Time == 90, "d90",
    ifelse(Time == 180, "d180",
      ifelse(Time == 360, "d360", "dUU")
    )
  )) %>%
  filter(sample_id != c("dUU")) %>%
  rename(
    age_id = Agebin,
    clone_id = Clone.ID,
    seq_count = Frequency
  ) %>%
  mutate(age_in_days = ifelse(sample_id == "d90", (900 - age_id) * 0.1,
    ifelse(sample_id == "d180", (1800 - age_id) * 0.1,
      (3600 - age_id) * 0.1
    )
  ))

memfast <- mem_fastfreq %>%
  mutate(sample_id = ifelse(Time == 90, "d90",
    ifelse(Time == 180, "d180",
      ifelse(Time == 360, "d360", "dUU")
    )
  )) %>%
  filter(sample_id != c("dUU")) %>%
  rename(
    age_id = Agebin,
    clone_id = Clone.ID,
    seq_count = Frequency
  ) %>%
  mutate(age_in_days = ifelse(sample_id == "d90", (900 - age_id) * 0.1,
    ifelse(sample_id == "d180", (1800 - age_id) * 0.1,
      (3600 - age_id) * 0.1
    )
  ))

memslow <- mem_slowfreq %>%
  mutate(sample_id = ifelse(Time == 90, "d90",
    ifelse(Time == 180, "d180",
      ifelse(Time == 360, "d360", "dUU")
    )
  )) %>%
  filter(sample_id != c("dUU")) %>%
  rename(
    age_id = Agebin,
    clone_id = Clone.ID,
    seq_count = Frequency
  ) %>%
  mutate(age_in_days = ifelse(sample_id == "d90", (900 - age_id) * 0.1,
    ifelse(sample_id == "d180", (1800 - age_id) * 0.1,
      (3600 - age_id) * 0.1
    )
  ))

# p1 <- ggplot(naiinc)+
#   geom_point(aes(x=log(clone_id), y=log(seq_count), col=age_id))
#
# p1 + facet_wrap(.~ Time)
#
# p1 <- ggplot(naiinc)+
#   geom_line(aes(x=log(clone_id), y=log(seq_count), col=age_id))
#
# p1 + facet_wrap(.~ Time)


# age dist
naidis %>%
  group_by(Time, age_id) %>%
  reframe(counts = sum(seq_count)) %>%
  # uncount(age_id)
  ggplot(aes(x = age_id, y = counts, col = factor(Time))) +
  geom_line() +
  geom_ribbon(aes(x = age_id, ymin = 0, ymax = counts, fill = factor(Time)), alpha = 0.2) +
  facet_wrap(. ~ Time)

naiinc %>%
  group_by(Time, age_id) %>%
  reframe(counts = sum(seq_count)) %>%
  # uncount(age_id)
  ggplot(aes(x = age_id, y = counts, col = factor(Time))) +
  geom_line() +
  geom_ribbon(aes(x = age_id, ymin = 0, ymax = counts, fill = factor(Time)), alpha = 0.2) +
  facet_wrap(. ~ Time)

memfast %>%
  group_by(Time, age_id) %>%
  reframe(counts = sum(seq_count)) %>%
  # uncount(age_id)
  ggplot(aes(x = age_id, y = counts, col = factor(Time))) +
  geom_line() +
  geom_ribbon(aes(x = age_id, ymin = 0, ymax = counts, fill = factor(Time)), alpha = 0.2) +
  facet_wrap(. ~ Time)

memslow %>%
  group_by(Time, age_id) %>%
  reframe(counts = sum(seq_count)) %>%
  # uncount(age_id)
  ggplot(aes(x = age_id, y = counts, col = factor(Time))) +
  geom_line() +
  geom_ribbon(aes(x = age_id, ymin = 0, ymax = counts, fill = factor(Time)), alpha = 0.2) +
  facet_wrap(. ~ Time, scales = "free")



# heatmap
memfast %>%
  arrange(desc(seq_count)) %>%
  filter(seq_count >= 10) %>%
  # uncount(seq_count)%>%
  ggplot() +
  geom_histogram(aes(x = (seq_count)), bins = 100)

naidis %>%
  arrange(desc(seq_count)) %>%
  filter(seq_count >= 10) %>%
  uncount(seq_count) %>%
  ggplot() +
  geom_histogram(aes(x = log(clone_id)), bins = 100)


memfast_top <- memfast %>%
  arrange(desc(seq_count)) %>%
  select(clone_id, seq_count)

memslow_top <- memslow %>%
  arrange(desc(seq_count)) %>%
  select(clone_id, seq_count)

memcor <- full_join(memfast_top, memslow_top, suffix = c(".fast", ".slow"))

# creating correlation matrix
corr_mat <- round(cor(memcor), 3)

# reduce the size of correlation matrix
melted_corr_mat <- melt(corr_mat)
head(melted_corr_mat)

# plotting the correlation heatmap
library(ggplot2)
ggplot(data = melted_corr_mat, aes(
  x = Var1, y = Var2,
  fill = value
)) +
  geom_tile()


### age contours density
naidis %>%
  filter(seq_count > 5) %>%
  group_by(Time, clone_id) %>%
  summarise("counts" = sum(seq_count)) %>%
  ggplot(aes(x = log(clone_id), y = log(counts))) +
  stat_density2d(aes(fill = stat(level)), geom = "polygon") +
  scale_fill_gradient(low = "lightskyblue1", high = "darkred") +
  theme(legend.position = "none") +
  labs(
    y = "Log(Counts)",
    x = "Log(Clone ID)",
    title = "Clone distribution in naive Tregs"
  ) +
  facet_wrap(. ~ Time) +
  my_theme

memfast %>%
  filter(seq_count > 5) %>%
  group_by(Time, clone_id) %>%
  summarise("counts" = sum(seq_count)) %>%
  ggplot(aes(x = log(clone_id), y = counts)) +
  stat_density2d(aes(fill = stat(level)), geom = "polygon") +
  scale_fill_gradient(low = "lightskyblue1", high = "darkred") +
  theme(legend.position = "none") +
  labs(
    y = "Log(Counts)",
    x = "Log(Clone ID)",
    title = "Clone distribution in the fast subset of memory Tregs"
  ) +
  facet_wrap(. ~ Time) +
  xlim(10, 14.25) +
  ylim(2, 26) +
  my_theme

memslow %>%
  filter(seq_count > 5) %>%
  group_by(Time, clone_id) %>%
  summarise("counts" = sum(seq_count)) %>%
  ggplot(aes(x = log(clone_id), y = counts)) +
  stat_density2d(aes(fill = stat(level)), geom = "polygon") +
  scale_fill_gradient(low = "lightskyblue1", high = "darkred") +
  theme(legend.position = "none") +
  labs(
    y = "Log(Counts)",
    x = "Log(Clone ID)",
    title = "Clone distribution in the slow subset of memory Tregs"
  ) +
  facet_wrap(. ~ Time) +
  xlim(12.5, 14) +
  ylim(2, 18) +
  my_theme


naidis %>%
  filter(seq_count > 5) %>%
  group_by(Time, age_in_days) %>%
  summarise("counts" = sum(seq_count)) %>%
  ggplot(aes(x = age_in_days, y = (counts))) +
  geom_density_2d_filled(color = "lightblue", linewidth = 0.2) +
  guides(fill = "none") +
  facet_wrap(. ~ Time, scales = "free") +
  # scale_y_continuous(trans = "log10", breaks = fancy_scientific, minor_breaks = log10minorbreaks) +
  # ylim(4, 10) +
  labs(
    y = "Cell counts",
    x = "Cell age (days)",
    title = "Cell age distribution in naive Tregs"
  ) +
  my_theme

memfast %>%
  filter(seq_count > 5) %>%
  group_by(Time, age_in_days) %>%
  summarise("counts" = sum(seq_count)) %>%
  ggplot(aes(x = age_in_days, y = (counts))) +
  geom_density_2d_filled(color = "lightblue", linewidth = 0.2) +
  guides(fill = "none") +
  facet_wrap(. ~ Time, scales = "free") +
  # ylim(3, 10) +
  xlim(0, 10) +
  labs(
    y = "Cell counts",
    x = "Cell age (days)",
    title = "Cell age distribution in the fast subset of memory Tregs"
  ) +
  my_theme

memslow %>%
  filter(seq_count > 5) %>%
  group_by(Time, age_in_days) %>%
  summarise("counts" = sum(seq_count)) %>%
  ggplot(aes(x = age_in_days, y = (counts))) +
  geom_density_2d_filled(color = "lightblue", linewidth = 0.2) +
  guides(fill = "none") +
  facet_wrap(. ~ Time, scales = "free") +
  # ylim(3, 10) +
  xlim(0, 10) +
  labs(
    y = "Cell counts",
    x = "Cell age (days)",
    title = "Cell age distribution in the slow subset of memory Tregs"
  ) +
  my_theme

## 2D contour
clones <- countClones(naidis, group = "sample_id")
head(clones, 5)

curve <- estimateAbundance(naidis, group = "sample_id", ci = 0.95, nboot = 100, clone = "clone_id")
# Plots a rank abundance curve of the relative clonal abundances
sample_colors <- c("d90" = "seagreen", "d180" = "steelblue", "d270" = "magenta", "d360" = "darkred", "d450" = "purple")
plot(curve, colors = sample_colors, legend_title = "Sample") +
  my_theme

remove(clones)
remove(curve)
