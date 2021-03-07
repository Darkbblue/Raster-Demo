# Raster-Demo
光栅图形学算法演示

## 概况
光栅图形学经典算法的动态可视化演示，基于QT制作。  
算法参考清华出版社《计算机图形学基础教程(第2版)》。  

## 框架描述
在主窗口类中记录下所有需要绘制的元素，绘图事件根据记录进行绘制，核心功能通过设置绘制元素来间接绘制  
需要记录的元素：原始图形，活跃像素，辅助标记(用在像素边长中点处的标志)  
表面上绘制的像素边长为50个实际的像素，从而体现出光栅扫描转换的特征  
操作方法：输入待绘制图形的参数，并选择一个算法进行绘制  

## 当前发布版
ver 0.5  
提供除多边形扫描转换算法外的全部功能  
使用方法：先点击左上角的SetShape，再选择一种算法开启演示  
Poly#1和Poly#2暂时无法使用(预定为多边形扫描转换算法的入口)  
使用区域填充算法演示(Poly#3和Poly#4)时，请在完成多边形绘制后手动设置种子点  

## 更新记录
2021.3.7 提供win10平台的发布版
2021.3.6 完成区域填充算法  
2021.3.5 完成圆弧的扫描转换算法  
2021.3.4 完成直线段扫描转换算法  
2021.2.28 完成基本框架构建  
