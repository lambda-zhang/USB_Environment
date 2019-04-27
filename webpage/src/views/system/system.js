import environment from '@/components/environment/environment.vue'
import headerbar from '@/components/headerbar/headerbar.vue'
import footerbar from '@/components/footerbar/footerbar.vue'

export default {
  name: 'system',
  data () {
    return {
      tscpu: 0,
    }
  },
  components: {
      headerbar: headerbar,
      footerbar: footerbar,
      environment: environment
  },
  created () {
  },
  mounted(){
  },
  beforeDestroy () {
  },
  methods: {
  }
}
