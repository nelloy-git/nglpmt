const testAddon = require('./build/Debug/testaddon.node')

const {
    Worker, isMainThread, parentPort, workerData
} = require('node:worker_threads');

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

let used_id = [];
let esc_pressed = false;

async function demo(context) {
    return new Promise(async (resolve) => {
        let id = context.onKeyAdd("repeat", (key, scancode, action, mods) => {
            console.log(scancode, action);
            if (scancode == 1 && action == 0){
                esc_pressed = true
            }
        })
        used_id.push(id);

        if (isMainThread){
            let buffer = new testAddon.GlBuffer(context);
            buffer.data("GL_STATIC_READ", new Float32Array(1, 3, 5));
            let usage = buffer.getUsage();
            
            let buffer_2 = new testAddon.GlBuffer(context);
            buffer_2.data(usage, new Float32Array(1, 3, 5));
            
            console.log("Val_0: " + usage.value());
            while(!esc_pressed){
                await context.run();
            }
            console.log("Val_1: " + usage.value());
        }
        resolve(true);
    })
}

async function run(){
    let worker
    if (isMainThread) {
        worker = new Worker(__filename);
        await sleep(100);
    }
    let context = new testAddon.Context('Test');
    await demo(context);

    if (isMainThread) {
        console.log("jsTerminate");
        if (worker){
            worker.terminate();
        }

        context.onKeyClear();
        console.log("jsCleanup");

        // let quit = false;
        // event.addActionQueued("cond", (a1, a2, a3) => {
        //     console.log("Main: " + a1 + ", " + a2 + ", " + a3);
        //     return !quit;
        // })
        // quit = true;
        // console.log("jsPushed");
        // await event.emitQueued(0, 0, 0);
    }
}

run();