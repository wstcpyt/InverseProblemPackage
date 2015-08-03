//
// Created by yutong pang on 6/26/15.
//
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "LineChart.h"
void LineChart::drawchart(std::vector<double>& x, std::vector<double>& y, const int index) {
    size_t num_points = x.size();
    int i;
    FILE * temp = fopen("tempadata.temp", "w");
    /*Opens an interface that one can use to send commands as if they were typing into the
     *     gnuplot command line.  "The -persistent" keeps the plot open even after your
     *     C program terminates.
     */
    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
    for (i=0; i < num_points; i++)
    {
        fprintf(temp, "%lf %lf \n", x[i], y[i]); //Write the data to a temporary file
    }
    //fprintf(gnuplotPipe, "set term png \n");
    //fprintf(gnuplotPipe, "set output \"/Users/yutongpang/fdtdplot/fdtdplot-%d.png\" \n", index);
    //fprintf(gnuplotPipe, "set title \"Reconstructed Mass Density Distribution\" \n"); //Send commands to gnuplot one by one.
    //fprintf(gnuplotPipe, "set yrange [0.8:2.2] \n");
    fprintf(gnuplotPipe, "set style data points \n");
    fprintf(gnuplotPipe, "set xlabel \"s\" font \"Times-Roman, 16\" \n");
    fprintf(gnuplotPipe, "set ylabel \"Gravity field\" font \"Times-Roman, 16\" \n");
    fprintf(gnuplotPipe, "set xtics font \"Times-Roman, 14\" \n");
    fprintf(gnuplotPipe, "set ytics font \"Times-Roman, 14\" \n");
    fprintf(gnuplotPipe, "set key off \n");
    fprintf(gnuplotPipe, "set logscale x \n");
    fprintf(gnuplotPipe, "set logscale y \n");
    fprintf(gnuplotPipe, "plot 'tempadata.temp'\n");
    fclose(temp);
    pclose(gnuplotPipe);
}
