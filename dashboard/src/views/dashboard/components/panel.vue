<template>
  <div>
    <svg-icon icon-class="iota" class-name="iota-logo"/>
    <div v-if="root">
      <el-row v-loading="loading" :gutter="40" class="panel-group">
        <el-col :xs="6" :sm="6" :lg="6" class="card-panel-col">
          <div class="card-panel" @click="handleSetLineChartData('temp')">
            <div v-bind:class="calcTempColor" class="card-panel-icon-wrapper icon-thermometer">
              <svg-icon icon-class="thermometer" class-name="card-panel-icon"/>
            </div>
            <div class="card-panel-description">
              <div class="card-panel-text">Temperature</div>
              <count-to
                :start-val="0"
                :end-val="actual_temp"
                :duration="3000"
                class="card-panel-num"
              />
            </div>
          </div>
        </el-col>
        <el-col :xs="6" :sm="6" :lg="6" class="card-panel-col">
          <div class="card-panel" @click="handleSetLineChartData('hum')">
            <div v-bind:class="calcHumColor" class="card-panel-icon-wrapper icon-humidity">
              <svg-icon icon-class="humidity" class-name="card-panel-icon"/>
            </div>
            <div class="card-panel-description">
              <div class="card-panel-text">Humidity</div>
              <count-to
                :start-val="0"
                :end-val="actual_hum"
                :duration="3000"
                class="card-panel-num"
              />
            </div>
          </div>
        </el-col>

        <el-col :xs="12" :sm="6" :lg="6" class="card-panel-col">
          <div class="card-panel" @click="handleSetLineChartData('hum')">
            <div class="card-panel-icon-wrapper icon-humidity">
              <svg-icon icon-class="clock" class-name="card-panel-icon"/>
            </div>
            <div class="card-panel-description">
              <div class="card-panel-text">Last Time</div>
              <div class="card-panel-time">{{actual_time}}</div>
            </div>
          </div>
        </el-col>
      </el-row>

      <el-row>
        <el-col :span="24">
          <el-card>
            <h2>Latest Data</h2>
            <el-button @click="root = ''; input_root = '' " type="danger">Disconnect</el-button>
            <el-table :data="sortedData" style="width: 100%">
              <el-table-column prop="timestamp" label="timestamp" :formatter="timestampFormatter"></el-table-column>
              <el-table-column prop="temperature" label="temperature"></el-table-column>
              <el-table-column prop="humidity" label="humidity"></el-table-column>s
            </el-table>
          </el-card>
        </el-col>
      </el-row>
    </div>
    <div v-if="!root">
      <el-form :inline="true">
        <el-form-item label="MAM root:">
          <el-input placeholder="Please input MAM root" v-model="input_root"></el-input>
        </el-form-item>
        <el-form-item>
          <el-button @click="connectToMAM" type="success">Load data</el-button>
        </el-form-item>
      </el-form>
    </div>
  </div>
</template>

<script>
import CountTo from "vue-count-to";
const Mam = require("@iota/mam");
const moment = require("moment");
const { trytesToAscii } = require("@iota/converter");

const mode = "restricted";
const secretKey = "VERYSECRETKEY";
const provider = "https://nodes.devnet.iota.org";

// Initialise MAM State
let mamState = Mam.init(provider);

// Set channel mode
mamState = Mam.changeMode(mamState, mode, secretKey);

export default {
  name: "Panel",
  components: {
    CountTo
  },
  data() {
    return {
      input_root: "",
      root: "",
      loading: false,
      timer: "",
      data: [],
      actual_temp: 0,
      actual_hum: 0,
      actual_time: Date.now()
    };
  },
  methods: {
    connectToMAM() {
      this.root = this.input_root;
      this.fetchData();
      this.timer = setInterval(this.fetchData, 5000);
    },
    handleSetLineChartData(type) {
      console.log("Clicke, ", type);
    },
    fetchData: async function() {
      this.loading = true;

      // Output asyncronously using "logData" callback function
      if (this.data.length == 0) {
        let fillData = data => this.data.push(JSON.parse(trytesToAscii(data)));
        await Mam.fetch(this.root, mode, secretKey, fillData);
      } else {
        this.loading = false;
        let tempData = [];
        let addData = data => tempData.push(JSON.parse(trytesToAscii(data)));
        await Mam.fetch(this.root, mode, secretKey, addData);

        if (tempData.length != this.data) {
          this.data = tempData;
        }
      }
      this.actual_temp = parseInt(this.sortedData[0].temperature);
      this.actual_hum = parseInt(this.sortedData[0].humidity);
      this.actual_time = moment(this.sortedData[0].timestamp).format(
        "MMMM Do YYYY, h:mm:ss a"
      );
    },
    timestampFormatter(row, column) {
      return moment(row.timestamp).format("MMMM Do YYYY, h:mm:ss a");
    }
  },
  created: function() {},
  beforeDestroy() {
    clearInterval(this.timer);
  },
  computed: {
    sortedData: function() {
      function compare(a, b) {
        if (a.timestamp > b.timestamp) return -1;
        if (a.timestamp < b.timestamp) return 1;
        return 0;
      }
      return this.data.sort(compare);
    },
    calcHumColor: function() {
      if (this.actual_temp <= 15) {
        return "blue";
      } else if (this.actual_temp <= 25) {
        return "green";
      } else if (this.actual_temp <= 40) {
        return "yellow";
      } else {
        return "red";
      }
    },
    calcTempColor: function() {
      if (this.actual_temp <= 18) {
        return "blue";
      } else if (this.actual_temp < 25) {
        return "green";
      } else if (this.actual_temp <= 28) {
        return "yellow";
      } else {
        return "red";
      }
    }
  }
};
</script>

<style lang="scss" scoped>
.panel-group {
  margin-top: 18px;
  .card-panel-col {
    margin-bottom: 32px;
  }
  .card-panel {
    height: 108px;
    cursor: pointer;
    font-size: 12px;
    position: relative;
    overflow: hidden;
    color: #666;
    background: #fff;
    box-shadow: 4px 4px 40px rgba(0, 0, 0, 0.05);
    border-color: rgba(0, 0, 0, 0.05);
    &:hover {
      .card-panel-icon-wrapper {
        color: #fff;
      }
      .icon-people {
        background: #40c9c6;
      }
      .icon-message {
        background: #36a3f7;
      }
      .icon-money {
        background: #f4516c;
      }
      .icon-shopping {
        background: #34bfa3;
      }
    }
    .icon-people {
      color: #40c9c6;
    }
    .icon-message {
      color: #36a3f7;
    }
    .icon-money {
      color: #f4516c;
    }
    .icon-shopping {
      color: #34bfa3;
    }
    .card-panel-icon-wrapper {
      float: left;
      margin: 14px 0 0 14px;
      padding: 16px;
      transition: all 0.38s ease-out;
      border-radius: 6px;
    }
    .card-panel-icon {
      float: left;
      font-size: 48px;
    }
    .card-panel-description {
      float: right;
      font-weight: bold;
      margin: 26px;
      margin-left: 0px;
      .card-panel-text {
        line-height: 18px;
        color: rgba(0, 0, 0, 0.45);
        font-size: 16px;
        margin-bottom: 12px;
      }
      .card-panel-num {
        font-size: 20px;
      }
      .card-panel-time {
        font-size: 15px;
      }
    }
  }
  .red {
    color: red;
  }
  .yellow {
    color: yellow;
  }
  .green {
    color: green;
  }
  .blue {
    color: blue;
  }
}
.iota-logo {
  width: 150px;
  height: 150px;
}
</style>
