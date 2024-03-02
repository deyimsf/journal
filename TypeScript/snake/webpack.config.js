const path = require('path');

// 引入html插件(编译后让html和js绑定)  cnpm i -D html-webpack-plugin
const HTMLWebpackPlugin = require('html-webpack-plugin')
// 引入clean插件(每次编译后清除dist目录) cnpm i -D clean-webpack-plugin
const {CleanWebpackPlugin} = require('clean-webpack-plugin')


// webpack中的所有配置信息都应该写在module.exports中
module.exports = {
    mode: "development",

    // 指定入口文件
    entry: "./src/index.ts",

    // 指定打包文件所在的目录
    output: {
        // 指定打包文件的目录
        path: path.resolve(__dirname, 'dist'),
        // 打包后文件名
        filename: "bundle.js",

        // 告诉webpack不要使用箭头函数 (req)=>{}
        environment: {
            arrowFunction: false
        }
    },

    // 指定webpack打包时要使用的模块
    module: {
        // 指定加载规则
        rules: [
            {
                // 指定需要处理的文件
                test: /\.ts$/, //所有以ts结尾的文件
                // 用哪个loader处理上面的文件
                use: [
                    // 配置babel(做兼容用)
                    {
                        // 指定加载器
                        loader: "babel-loader",
                        // 设置babel
                        options: {
                            // 设置预定义环境(代码要在哪些浏览器运行)
                            presets:[
                                [
                                    // 指定环境插件
                                    "@babel/preset-env",
                                    // 配置信息
                                    {
                                        // 指定要兼容的浏览器版本
                                        targets:{
                                            "chrome": "58",
                                            "ie": "11"
                                        },
                                        // 指定corejs的版本
                                        "corejs":"3",
                                        // 使用corejs的方式  usage:按需加载
                                        "useBuiltIns": "usage"
                                    }
                                ]
                            ]
                        }
                    },
                    'ts-loader'
                ], //loader 执行顺序是从后往前
                // 要排除的文件
                exclude: '/node_modules/'
            },

            // 设置对less文件的处理
            {
                test: /\.less$/,
                use: [
                    "style-loader",
                    "css-loader",
                    {
                        loader: "postcss-loader",
                        options: {
                            postcssOptions: {
                                plugins: [
                                    [
                                        "postcss-preset-env",
                                        {
                                            //兼容两个最新版本的浏览器
                                            browsers: 'last 2 versions'
                                        }
                                    ]
                                ]
                            }
                        }
                    },
                    "less-loader"
                ]
            }
        ]
    },

    // 配置webpack插件
    plugins: [
        new CleanWebpackPlugin(),
        new HTMLWebpackPlugin({
            // title: "自定义首页title"
            // 自定义首页模版文件位置
            template: "./src/index.html"
        })
    ],

    // 用来设置那些文件可以做作为模块引用
    resolve: {
        extensions: ['.ts','.js']
    }
}