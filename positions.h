const int middle_id = 1;
const int left_id = 2;
const int right_id = 3;
const int claw_id = 4;

const int a_pickup[][2] = { { middle_id, 40 }, { claw_id, 0 }, { left_id, 50 }, { right_id, 160 }, { claw_id, 90 }, { right_id, 90 }, { middle_id, 83 } };
const int a_drop[][2] = { { middle_id, 40 }, { left_id, 55 }, { right_id, 150 }, { claw_id, 0 }, { right_id, 90 }, { claw_id, 110 }, { middle_id, 83 } };

const int b_pickup[][2] = { { middle_id, 83 }, { claw_id, 0 }, { left_id, 50 }, { right_id, 160 }, { claw_id, 90 }, { right_id, 90 }, { middle_id, 83 } };
const int b_drop[][2] = { { middle_id, 83 }, { left_id, 55 }, { right_id, 150 }, { claw_id, 0 }, { right_id, 90 }, { claw_id, 110 }, { middle_id, 83 } };

const int c_pickup[][2] = { { middle_id, 132 }, { claw_id, 0 }, { left_id, 50 }, { right_id, 160 }, { claw_id, 90 }, { right_id, 90 }, { middle_id, 83 } };
const int c_drop[][2] = { { middle_id, 132 }, { left_id, 55 }, { right_id, 150 }, { claw_id, 0 }, { right_id, 90 }, { claw_id, 110 }, { middle_id, 83 } };

const int nothing_found[][2] = { { middle_id, 60 }, { middle_id, 100 }, { middle_id, 60 }, { middle_id, 100 }, { middle_id, 60 }, { middle_id, 83 } };
