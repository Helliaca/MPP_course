#define shortpress_tolerance 1000
#define longpress_tolerance 2000
#define doublepress_tolerance 800

uint8_t k1_d = 0, k1_u = 0, k2_d = 0, k2_u = 0;
uint8_t k1_d_t = -100000, k1_u_t = -100000, k2_d_t = -100000, k2_u_t = -100000;


// Return 1 if the key is currently in a pressed state
uint8_t get_key1() {
	return !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
}
uint8_t get_key2() {
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
}


// Return 1 only on the frame the key was pushed down
uint8_t key1_down() { return k1_d; }
uint8_t key2_down() { return k2_d; }

// Return 1 only on the frame the key was released
uint8_t key1_up() { return k1_u; }
uint8_t key2_up() { return k2_u; }

// Return 1 on the frame a key is released if the key was pressed for less time than shortpress_tolerance
uint8_t key1_shortpress() {
  return key1_up && k1_u_t - k1_d_t < shortpress_tolerance;
}
uint8_t key2_shortpress() {
  return key2_up && k2_u_t - k2_d_t < shortpress_tolerance;
}

// Return 1 on the frame a key is released if the key was pressed for longer than shortpress_tolerance, but less than longpress_tolerance
uint8_t key1_longpress() {
  return key1_up && k1_u_t - k1_d_t >= shortpress_tolerance && k1_u_t - k1_d_t < longpress_tolerance;
}
uint8_t key2_longpress() {
  return key2_up && k2_u_t - k1_d_t >= shortpress_tolerance && k2_u_t - k2_d_t < longpress_tolerance;
}

// Return 1 on the frame a key is pressed down if the key was released shortly earlier
uint8_t key1_doublepress() {
  return key1_down && k1_u_t + doublepress_tolerance < k1_d_t;
}
uint8_t key2_doublepress() {
  return key2_down && k2_u_t + doublepress_tolerance < k2_d_t;
}


void update_keys(uint8_t time) {
  static uint8_t k1_was_pressed = 0;
  static uint8_t k2_was_pressed = 0;

  k1_d = k1_was_pressed < get_key1();
  k2_d = k2_was_pressed < get_key2();

  k1_u = k1_was_pressed > get_key1();
  k2_u = k2_was_pressed > get_key2();

  if(k1_d) k1_d_t = time;
  if(k2_d) k2_d_t = time;
  if(k1_u) k1_u_t = time;
  if(k2_u) k2_u_t = time;

  k1_was_pressed = get_key1();
  k2_was_pressed = get_key2();
}
