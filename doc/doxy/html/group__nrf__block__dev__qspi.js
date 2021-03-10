var group__nrf__block__dev__qspi =
[
    [ "nrf_block_dev_qspi.c", "nrf__block__dev__qspi_8c.html", null ],
    [ "nrf_block_dev_qspi_work_t", "structnrf__block__dev__qspi__work__t.html", [
      [ "cache_flushing", "structnrf__block__dev__qspi__work__t.html#a30667efdb051bd81213d61a02f0078fe", null ],
      [ "erase_required", "structnrf__block__dev__qspi__work__t.html#a958df309fab5fabe59af3004ab5be2b8", null ],
      [ "erase_unit_dirty_blocks", "structnrf__block__dev__qspi__work__t.html#a61f4a53ee9c2889e7ef8eba509a35517", null ],
      [ "erase_unit_idx", "structnrf__block__dev__qspi__work__t.html#a0f1bae8e12dd6e2cad7693df3e64c40e", null ],
      [ "ev_handler", "structnrf__block__dev__qspi__work__t.html#a1086cae4afd46edf6aa3472b803096d9", null ],
      [ "geometry", "structnrf__block__dev__qspi__work__t.html#a5207bbb975b62c6d3e2f914069fdc9d4", null ],
      [ "left_req", "structnrf__block__dev__qspi__work__t.html#ac47a5aa8e1ea6f1a351f5dedc3e1f53e", null ],
      [ "p_context", "structnrf__block__dev__qspi__work__t.html#acecc2c6e8c7e8d1fcbc08069ba6bfa12", null ],
      [ "p_erase_unit_buff", "structnrf__block__dev__qspi__work__t.html#a0aa358f98fc67da059148c0270ebd469", null ],
      [ "req", "structnrf__block__dev__qspi__work__t.html#a77d79b5138b09c43f636f4442e1328d2", null ],
      [ "state", "structnrf__block__dev__qspi__work__t.html#a800ba66b6c856545fc2592d6298ef62f", null ],
      [ "writeback_mode", "structnrf__block__dev__qspi__work__t.html#a60a5916f7194f103bc0a55f30c628083", null ]
    ] ],
    [ "nrf_block_dev_qspi_config_t", "structnrf__block__dev__qspi__config__t.html", [
      [ "block_size", "structnrf__block__dev__qspi__config__t.html#ade40f4718c37b748c640a08bc3e71e16", null ],
      [ "flags", "structnrf__block__dev__qspi__config__t.html#a15a9cf79556bcd7921ff7e1172b90261", null ],
      [ "qspi_config", "structnrf__block__dev__qspi__config__t.html#ac5b30e5f45c35ec165767b694cdab1a7", null ]
    ] ],
    [ "nrf_block_dev_qspi_t", "structnrf__block__dev__qspi__t.html", [
      [ "block_dev", "structnrf__block__dev__qspi__t.html#a6cba607aa629e79db93f4895e301143d", null ],
      [ "info_strings", "structnrf__block__dev__qspi__t.html#af95da92e0abc96429f01b9e61314a3a5", null ],
      [ "p_work", "structnrf__block__dev__qspi__t.html#a2ba167e6f14bb2a485a8c7a85f786d3e", null ],
      [ "qspi_bdev_config", "structnrf__block__dev__qspi__t.html#a7c2c6eef234dbadbbe46e9445bc1ac04", null ]
    ] ],
    [ "NRF_BLOCK_DEV_QSPI_CONFIG", "group__nrf__block__dev__qspi.html#ga53f85a936743e96062545fa84042a7e3", null ],
    [ "NRF_BLOCK_DEV_QSPI_DEFINE", "group__nrf__block__dev__qspi.html#gacc0b782ed39b2feb37cd89c6826d0f0e", null ],
    [ "NRF_BLOCK_DEV_QSPI_ERASE_UNIT_SIZE", "group__nrf__block__dev__qspi.html#ga2ae6d6aa6548767fb84cc7faeb132c10", null ],
    [ "nrf_block_dev_qspi_flag_t", "group__nrf__block__dev__qspi.html#ga474ee5444c6145162016b694b70a62d9", [
      [ "NRF_BLOCK_DEV_QSPI_FLAG_CACHE_WRITEBACK", "group__nrf__block__dev__qspi.html#gga474ee5444c6145162016b694b70a62d9a830662caf58cc4c8a0a233ba9ede0b94", null ]
    ] ],
    [ "nrf_block_dev_qspi_state_t", "group__nrf__block__dev__qspi.html#gad2db3cde61b7ba777ca862fa1f0609db", [
      [ "NRF_BLOCK_DEV_QSPI_STATE_DISABLED", "group__nrf__block__dev__qspi.html#ggad2db3cde61b7ba777ca862fa1f0609dbaafdd305a30c2a339dd52e697227fdd7b", null ],
      [ "NRF_BLOCK_DEV_QSPI_STATE_IDLE", "group__nrf__block__dev__qspi.html#ggad2db3cde61b7ba777ca862fa1f0609dba3b369e2284959de9df77419f3b04753e", null ],
      [ "NRF_BLOCK_DEV_QSPI_STATE_READ_EXEC", "group__nrf__block__dev__qspi.html#ggad2db3cde61b7ba777ca862fa1f0609dba6596589051a3e3a8c6c252fa6afd693a", null ],
      [ "NRF_BLOCK_DEV_QSPI_STATE_EUNIT_LOAD", "group__nrf__block__dev__qspi.html#ggad2db3cde61b7ba777ca862fa1f0609dbac0e4de5060c1aca8f117387f6a1dcb5b", null ],
      [ "NRF_BLOCK_DEV_QSPI_STATE_WRITE_ERASE", "group__nrf__block__dev__qspi.html#ggad2db3cde61b7ba777ca862fa1f0609dbae5eefc2fc2be29380cc32f15832f0f45", null ],
      [ "NRF_BLOCK_DEV_QSPI_STATE_WRITE_EXEC", "group__nrf__block__dev__qspi.html#ggad2db3cde61b7ba777ca862fa1f0609dba87695a4e291d1ad5bd398a13342850f1", null ]
    ] ],
    [ "nrf_block_device_qspi_ops", "group__nrf__block__dev__qspi.html#ga7eec21240e901803f6fe380a778e70aa", null ]
];