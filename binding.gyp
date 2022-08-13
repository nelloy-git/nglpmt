{
    "targets": [{
        "target_name": "testaddon",
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        "sources": [
            "<!@(node -p \"require('fs').readdirSync('./include').map(f=>'include/'+f).join(' ')\")",
            "<!@(node -p \"require('fs').readdirSync('./src').map(f=>'src/'+f).join(' ')\")",
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")"
        ],
        'libraries': [],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }]
}