<template>
  <div class="dashboard-container">
    <h1> Universal Basic Nutrition Dashboard</h1>
    <Panel />
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
