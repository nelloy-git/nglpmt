const testAddon = require('./build/Debug/testaddon.node')

const {
    Worker, isMainThread, parentPort, workerData
} = require('node:worker_threads');

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

let used_id = [];

async function demo(event) {
    return new Promise(async (resolve) => {
        let name = isMainThread ? 'Main' : 'Worker';
        for (let i = 0; i < 5; i++) {
            console.log("JS loop " + name + ": " + i);

            let id = event.addActionQueued("repeat", (a1, a2, a3) => {
                console.log(name + ": " + a1 + ", " + a2 + ", " + a3);
            })
            used_id.push(id);

            if (isMainThread){
                console.log("JS emit");
                await event.emitQueued(i, i / 10);
                await sleep(1);
            } 
        }
        resolve(true);
    })
}

async function run(){
    let worker
    if (isMainThread) {
        // worker = new Worker(__filename);
        await sleep(70);
    }
    let event = new testAddon.Event('Test');
    await demo(event);

    if (isMainThread) {
        console.log("jsTerminate");
        // worker.terminate();

        for (var i = 0; i < used_id.length; i++) {
            event.delAction(used_id[i]);
        }
        console.log("jsCleanup");

        event.addActionQueued("once", (a1, a2, a3) => {
            console.log("Main: " + a1 + ", " + a2 + ", " + a3);
        })
        console.log("jsPushed");
        await event.emitQueued(0, 0, 0);
    }
}

run();