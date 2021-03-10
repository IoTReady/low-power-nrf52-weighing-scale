var group__nrf__serial =
[
    [ "nrf_serial_queues_t", "structnrf__serial__queues__t.html", [
      [ "p_rxq", "structnrf__serial__queues__t.html#a76b27996ea9eb6005f056b9a56004d9e", null ],
      [ "p_txq", "structnrf__serial__queues__t.html#a213f16b321f2e9a331fdbda82291c5ed", null ]
    ] ],
    [ "nrf_serial_buffers_t", "structnrf__serial__buffers__t.html", [
      [ "p_rxb", "structnrf__serial__buffers__t.html#a613d2009819e0751680b359561f27868", null ],
      [ "p_txb", "structnrf__serial__buffers__t.html#af1dc30dc7a9587ef572a8ce6e8ebe216", null ],
      [ "rx_size", "structnrf__serial__buffers__t.html#a25370552837fc201352b9923cfd60ef1", null ],
      [ "tx_size", "structnrf__serial__buffers__t.html#a2fd8df50c787b115a913aca9ac54b8aa", null ]
    ] ],
    [ "nrf_serial_config_t", "structnrf__serial__config__t.html", [
      [ "ev_handler", "structnrf__serial__config__t.html#a7d5d64cf62eba10b5f420aa8285399d3", null ],
      [ "mode", "structnrf__serial__config__t.html#a2f08647c1db900484ec9ee8f619e1246", null ],
      [ "p_buffers", "structnrf__serial__config__t.html#a77a30f0836ced54484ec62b25d32867f", null ],
      [ "p_queues", "structnrf__serial__config__t.html#a06551621ae5ebf6bce2aefbfbc637d3a", null ],
      [ "sleep_handler", "structnrf__serial__config__t.html#a3c8c80a51135cf2d9ba810d056e5d0c7", null ]
    ] ],
    [ "nrf_serial_ctx_t", "structnrf__serial__ctx__t.html", [
      [ "flags", "structnrf__serial__ctx__t.html#a4aab87d21efcb580c63da98cbe25df50", null ],
      [ "p_config", "structnrf__serial__ctx__t.html#acce4f1f83ef5d301634bf11e677da7fd", null ],
      [ "read_lock", "structnrf__serial__ctx__t.html#af7140a7681036f7bf3e01b54918f3a72", null ],
      [ "write_lock", "structnrf__serial__ctx__t.html#a0c37242172c2d8d247e6e50d6a40d03d", null ]
    ] ],
    [ "nrf_serial_s", "structnrf__serial__s.html", [
      [ "instance", "structnrf__serial__s.html#a31542a98d279b2ab9ec1c7234e5e6bdc", null ],
      [ "p_ctx", "structnrf__serial__s.html#a72b5f076e15a22228007090dea35ea80", null ],
      [ "p_rx_timer", "structnrf__serial__s.html#a957e483945799078dd7e0485e659f836", null ],
      [ "p_tx_timer", "structnrf__serial__s.html#af9fcbdb34e917c3995ea35525a5007ce", null ]
    ] ],
    [ "NRF_SERIAL_BUFFERS_DEF", "group__nrf__serial.html#ga0195d5e9660413084d03965fd387df07", null ],
    [ "NRF_SERIAL_CONFIG_DEF", "group__nrf__serial.html#ga2fa22c0832160f75d69f71deaa33a345", null ],
    [ "NRF_SERIAL_DRV_UART_CONFIG_DEF", "group__nrf__serial.html#ga3bf94929d2852dde706e3bf638165dec", null ],
    [ "NRF_SERIAL_MAX_TIMEOUT", "group__nrf__serial.html#ga819ff158f8268c49c4df3035aceac7b7", null ],
    [ "NRF_SERIAL_QUEUES_DEF", "group__nrf__serial.html#ga3b5e442f2c40c36bf08c639ea1f3c13b", null ],
    [ "NRF_SERIAL_RX_ENABLED_FLAG", "group__nrf__serial.html#ga36af4216265f4da6cedf76b830d4ce81", null ],
    [ "NRF_SERIAL_TX_ENABLED_FLAG", "group__nrf__serial.html#ga9f383f9b79e339757431a5c2fd9186e4", null ],
    [ "NRF_SERIAL_UART_DEF", "group__nrf__serial.html#ga262b5d9edd0a00a7adbe4a30452b954c", null ],
    [ "nrf_serial_evt_handler_t", "group__nrf__serial.html#ga38efa57bf85199767792f8cb58cff8a8", null ],
    [ "nrf_serial_sleep_handler_t", "group__nrf__serial.html#ga6f23292082c3f6e808f2730efd76a704", null ],
    [ "nrf_serial_t", "group__nrf__serial.html#ga2466c89a619269f90df88e0b1ae0a543", null ],
    [ "nrf_serial_event_t", "group__nrf__serial.html#ga95d927915f4db67d95e6cb9bd2a97d85", [
      [ "NRF_SERIAL_EVENT_TX_DONE", "group__nrf__serial.html#gga95d927915f4db67d95e6cb9bd2a97d85a3c92d406686706c5e6f1c9af8a7ab3e0", null ],
      [ "NRF_SERIAL_EVENT_RX_DATA", "group__nrf__serial.html#gga95d927915f4db67d95e6cb9bd2a97d85af470c5b4680d63791d328598fa9b40a8", null ],
      [ "NRF_SERIAL_EVENT_DRV_ERR", "group__nrf__serial.html#gga95d927915f4db67d95e6cb9bd2a97d85ac4d101e2964ccbc82d5653717e522bef", null ],
      [ "NRF_SERIAL_EVENT_FIFO_ERR", "group__nrf__serial.html#gga95d927915f4db67d95e6cb9bd2a97d85a9960f08aff4b3c1d377060724e9beee5", null ]
    ] ],
    [ "nrf_serial_mode_t", "group__nrf__serial.html#ga28d42a6ff602c49e23e9cf1b85d1df8b", [
      [ "NRF_SERIAL_MODE_POLLING", "group__nrf__serial.html#gga28d42a6ff602c49e23e9cf1b85d1df8ba2a369e6eadd464dc5d90bc1e7d4864e6", null ],
      [ "NRF_SERIAL_MODE_IRQ", "group__nrf__serial.html#gga28d42a6ff602c49e23e9cf1b85d1df8baecb0c243b92f9c6c4eb2c3bf1e432e0b", null ],
      [ "NRF_SERIAL_MODE_DMA", "group__nrf__serial.html#gga28d42a6ff602c49e23e9cf1b85d1df8ba1f09af9e1d083f6430cb931314ae668c", null ]
    ] ],
    [ "nrf_serial_flush", "group__nrf__serial.html#ga4912baad1dcc3d6fa4f05ad7b5acadbb", null ],
    [ "nrf_serial_init", "group__nrf__serial.html#gab07723891b8531eecfdbd90498ffabf2", null ],
    [ "nrf_serial_read", "group__nrf__serial.html#ga3402c8874c264174a54fd966e8a7d9d3", null ],
    [ "nrf_serial_rx_drain", "group__nrf__serial.html#ga16e23a5364f4c1f22f9abedc5b20b620", null ],
    [ "nrf_serial_tx_abort", "group__nrf__serial.html#gaa6ce920eb8f077633d7f7a1d7d790ce0", null ],
    [ "nrf_serial_uninit", "group__nrf__serial.html#ga5131c92d69f2b6d59f1f7d86834debb9", null ],
    [ "nrf_serial_write", "group__nrf__serial.html#ga429b73699af2e725894aa95c88778964", null ]
];