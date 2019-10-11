library(ggplot2)
ggplot(q3_3, aes(x = start, y = number)) + geom_bar(stat = "identity", width = 0.5) + scale_x_discrete(limits=c("1-2", "2-3", "3-4", "4-5", "5-6", "6-7", "7-8", "8-9", "9-10", "10-11", "11-12", "12-13", "13-14", "14-15", "15-16", "16-17", "17-18", "18-19", "19-20", "20-21", "21-22", "22-23", "23-24"))



# install R, then R Studio and then ggplot2
