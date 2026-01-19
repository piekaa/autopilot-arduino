#ifndef IC2MULTIPLEXER_H
#define IC2MULTIPLEXER_H

class IC2Multiplexer {

public:
  IC2Multiplexer() {
    Wire1.begin(15, 27); // SDA, SCL
  }

  void selectChannel(uint8_t channel) {
    Wire1.beginTransmission(0x70);
    Wire1.write(1 << channel);
    Wire1.endTransmission();
  }
};

#endif