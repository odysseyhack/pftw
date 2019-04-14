const http = require('http');

const { run, send } = require('micro');

const PORT = process.env.PORT || 3443;

var aedes = require('aedes')()
var mqtt_server = require('net').createServer(aedes.handle)
var mqtt_port = 1337

var lastest_data = {}

var data_fetched = false

var status = "temperature is okay"

mqtt_server.listen(mqtt_port, function () {
    console.log('mqtt server listening on port', mqtt_port)
    
})
aedes.subscribe('sensor', (packet, cb) => {
    console.log("sensor: ", packet.payload.toString());
    lastest_data = JSON.parse(packet.payload.toString());
    data_fetched = true

    if (lastest_data.temperature > 25) {
        status = "temperature is high!"
    }
});

const microHttps = fn => http.createServer((req, res) => run(req, res, fn));

let root = ''

const server = microHttps(async (req, res) => {
    res.setHeader('Content-Type', 'application/json');
    let obj = {
        status: status,
        lastest_data: lastest_data,
        root: rootArray[0]
    }
    res.end(JSON.stringify(obj));
});

const Mam = require('@iota/mam')
const { asciiToTrytes, trytesToAscii } = require('@iota/converter')

const mode = 'restricted'
const secretKey = 'VERYSECRETKEY'
const provider = 'https://nodes.devnet.iota.org'

let rootArray = []
// Initialise MAM State
let mamState = Mam.init(provider)

// Set channel mode
mamState = Mam.changeMode(mamState, mode, secretKey)

var CronJob = require('cron').CronJob;
new CronJob('0 */1 * * * *', function () {

    if (data_fetched) {
        let timestamp = Date.now();


        // get the correct data here
        let data = {
            timestamp: timestamp,
            temperature: lastest_data.temperature,
            humidity: lastest_data.humidity
        }
        console.log('Publish data');
        root = publish(data);
    } else {
        console.log('No sensor data fetched yet.');
    }
    
}, null, true, 'America/Los_Angeles');

server.listen(PORT);
console.log(`Listening on http://localhost:${PORT}`);

// Publish to tangle
const publish = async packet => {
    // Create MAM Payload - STRING OF TRYTES
    const trytes = asciiToTrytes(JSON.stringify(packet))
    const message = Mam.create(mamState, trytes)

    // Save new mamState
    mamState = message.state

    // Attach the payload
    await Mam.attach(message.payload, message.address, 3, 9)

    console.log('Published', packet, '\n');
    console.log("root ", message.root)
    rootArray.push(message.root)

    return message.root
}