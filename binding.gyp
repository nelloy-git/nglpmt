{
    "targets": [{
        "target_name": "testaddon",
        "cflags!": [ ],
        "cflags_cc!": [ ],
        "cflags_cc": ['-fexceptions', '-fconcepts', '-std=c++2a'],
        "sources": [
            "<!@(node -p \"var fs=require('fs'),path=require('path'),walk=function(r){let t,e=[],n=null;try{t=fs.readdirSync(r)}catch(r){n=r.toString()}if(n)return n;var a=0;return function n(){var i=t[a++];if(!i)return e;let u=path.resolve(r,i);i=r+'/'+i;let c=fs.statSync(u);if(c&&c.isDirectory()){let r=walk(i);return e=e.concat(r),n()}return e.push(i),n()}()};walk('./src').join(' ');\")",
            "3rd_party/glad/src/gl.c"
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")",
            "3rd_party/glad/include",
            "3rd_party/glfw_3.3.8/include",
            "3rd_party/vld/include",
            "3rd_party/thread-pool",
            "src"
        ],
        'conditions': [
          ['OS=="linux"', {
            'libraries': [
              '-lglfw',
            ],
          }],
          ['OS=="win"', {
            'libraries': [
                "<(module_root_dir)/3rd_party/glfw_3.3.8/lib-vc2022/glfw3dll.lib",
                "<(module_root_dir)/3rd_party/vld/vld.lib"
            ],
          }]
        ],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ 'NGLPMT_DEBUG' ],
        'msvs_settings': {
            'VCCLCompilerTool': { "ExceptionHandling": 1, 'AdditionalOptions': [ '-std:c++20', '-W3'] }
        },
    }]
}