module.exports = {
    plugins: [
        'react-hot-loader/babel',
        '@babel/plugin-transform-runtime',
        '@babel/plugin-proposal-class-properties',
        '@babel/plugin-proposal-export-default-from',
        '@babel/plugin-proposal-optional-chaining'
    ],
    presets: [
        '@babel/preset-env',
        'babel-preset-minify',
        '@babel/preset-react'
    ]
};