/* eslint-env node */
const {resolve} = require('path');
const DashboardPlugin = require('webpack-dashboard/plugin');
const TerserPlugin = require('terser-webpack-plugin');
const CopyPlugin = require('copy-webpack-plugin');

module.exports = (env, argv) => ({
    mode: argv.mode === 'production' ? 'production' : 'development',
    entry: [
        ...(argv.mode === 'production' ? [] : ['react-hot-loader/patch']),
        './src/App.bs.js'
    ],
    devtool: (argv.mode === 'production') ? void 0 : 'eval-source-map',
    output: {
        path: resolve('./dist'),
        filename: 'bundle.min.js'
    },
    context: __dirname,
    devServer: {
        port: 4669,
        host: '0.0.0.0',
        contentBase: './dist',
        compress: true,
        watchContentBase: true
    },
    /*
    module: {
        rules: [
            {
                test: /.jsx?$/,
                exclude: /node_modules/,
                loader: 'babel-loader',
                query: {
                    presets: [
                        '@babel/env'
                    ]
                }
            }
        ]
    },
    */
    optimization: {
        minimize: argv.mode === 'production',
        minimizer: [new TerserPlugin()]
    },
    plugins: [
        new DashboardPlugin(),
        new CopyPlugin([
            { from: "assets/**/*", to: "dist/", toType: "dir" }
        ])
    ],
    resolve: {
        mainFields: ['module', 'main'],
        modules: [resolve(__dirname, './src'), 'node_modules'],
        extensions: ['.js'],
        /*
        alias: {
             'react-dom': '@hot-loader/react-dom'
        }
        */
    }
});
