# tinyEasyMuduoWebServer
This project is a tiny easy web server which is based on linux QtCreator C++, muduo, jsoncpp, MySQL and manually implemented HTTP protocol.

# required dependencies
You need to execute the following commands to install the required dependencies.

```sh
sudo apt-get install libjsoncpp-dev
sudo apt-get install mysql-server mysql-client
```

Then you need to configure your database information so that the program can interact with your database normally.
[configure MySQL](https://www.cnblogs.com/wuhou/archive/2008/09/28/1301071.html)

Finally, you need to install static library muduo.
[configure muduo](https://github.com/chenshuo/muduo)

# database table structure
|id |license_plate |time |ip |
|---|---|---|---|
|int|varchar(20)|timestamp|varchar(20)|

# run
Configure the .pro file to make sure your libs can link to the program and build in QtCreater.

After building the project you need to put the background image in the root directory of the build project.
