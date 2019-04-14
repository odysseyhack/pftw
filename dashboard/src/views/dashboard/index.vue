<template>
  <div class="dashboard-container">
    <h1> Universal Basic Nutrition Dashboard</h1>
    <Panel />
    <el-row>
      <el-col :span="24">
        <el-card>
          <h2>Phases</h2>
          <ul>
            <li>Phase 1: Growing</li>
            <ol>
              <li>Input eggs</li>
              <li>Wait</li>
              <li>Input feed</li>
              <li>Wait</li>
            </ol>
            <li>Phase 2: Harvest</li>
            <ol>
              <li>Open doors</li>
              <li>Start attratic them</li>
              <li>count outgooing worms</li>
              <li>close doors when 95% of worms leave</li>
            </ol>
            <li>Phase 3: Evolution</li>
            <ol>
              <li>Feed again</li>
              <li>Wait that worms become beatles</li>
              <li>Wait for eggs</li>
            </ol>
            <li>Phase 4: Harvest</li>
            <ol>
              <li>Open doors</li>
              <li>Start attratic them</li>
              <li>count outgooing beatles</li>
              <li>close doors when all the beatles are outside</li>
            </ol>
            <li>Repeat</li>
          </ul>
        </el-card>
      </el-col>
    </el-row>
  </div>
</template>

<script>
import Panel from './components/Panel'

export default {
  name: "Dashboard",
  components: {
    Panel
  },
  data() {
    return {    };
  }
};
</script>

<style rel="stylesheet/scss" lang="scss" scoped>
.dashboard {
  &-container {
    margin: 30px;
  }
  &-text {
    font-size: 30px;
    line-height: 46px;
  }
}

</style>
