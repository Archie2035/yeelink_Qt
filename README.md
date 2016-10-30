# yeelink_Qt
这是一个使用Qt编写的Demo，用于使用yeelink提供的API接口在本地客户端与yeelink服务端进行交互。
yeelink提供了6种类型的传感器供我们使用，其中除了微博类型外其余5种类型均在次项目中进行了尝试，每种传感器包含了上传数据和获取服务器最后数据的操作。</br>
yeelink的api文档地址：[点击这里](http://www.yeelink.net/develop/api) </br>
我的[设备地址](http://www.yeelink.net/devices/351739)
![](http://img.blog.csdn.net/20161030224646601)
*注*本人为智能家居爱好者，一直在业余时间致力于智能家居系统的调研与实践，与yeelink公司无任何利益相关。

# 开始
## 创建传感器
- 注册yeelink[地址](http://developer.yeelink.net/zh_CN/)
- 添加新设备
- 添加传感器   6种类型选择

使用本项目中提供的库玩转“数值类型”传感器，在上面创建完传感器后到[账户设置](http://developer.yeelink.net/zh_CN/member/apikey)中拿到apikey。
将`yeelinkapi_v1.h`,`yeelinkapi_v1.cpp`两个文件添加到你的Qt项目中。

## “数值类型”传感器为例
1.  创建一个传感器对象：`yeelinkValueSensor = new YeelinkValueTypeSensor();`
2.  初始化设备地址、传感器地址、appKey：`yeelinkValueSensor->init(deviceId,valueSensorId,appKey);`
3.  连接传感器返回数据的信号： `connect(yeelinkValueSensor,SIGNAL(sig_valData(double)),this,SLOT(slt_valData(double)));`
4.  构建数据点上传数据：`yeelinkValueSensor->setValTypeData(12);`
具体使用方法详见项目中Demo目录下的Qt工程。

## 发现问题
可以提issue或者[留言](http://blog.csdn.net/tianzhihen_wq/article/details/52974107)

## 敬请期待
近期提供nodejs版客户端的实现，以提供树莓派爱好者方便将数据上传至yeelink

# showtime
###数值型传感器界面
![](https://github.com/tianzhihen/yeelink_Qt/blob/master/img/c%E6%95%B0%E5%80%BC%E5%9E%8B.png?raw=true)

###开关型传感器界面
![](https://github.com/tianzhihen/yeelink_Qt/blob/master/img/c%E5%BC%80%E5%85%B3%E5%9E%8B.png?raw=true)

###GPS型传感器界面
![](https://github.com/tianzhihen/yeelink_Qt/blob/master/img/cGPS%E5%9E%8B.png?raw=true)

###泛型传感器界面
![](https://github.com/tianzhihen/yeelink_Qt/blob/master/img/c%E6%B3%9B%E5%9E%8B.png?raw=true)

###图片型传感器界面
![](https://github.com/tianzhihen/yeelink_Qt/blob/master/img/c%E5%9B%BE%E7%89%87%E5%9E%8B.png?raw=true)
