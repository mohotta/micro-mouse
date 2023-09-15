
int min_dir(int* direction_values) {
  int min = 0;
  for (int i = 1; i < 4; i++) {
    if (direction_values[min] > direction_values[i])
      min = i;
  }
  return min;
}

// sensor values 1-wall 0-no disturbance
// mapping surrouding walls by sensor data
int* map_maze(
    int** maze,
    int x_position,
    int y_position,
    bool left_sensor,
    bool right_sensor,
    bool front_sensor,
    int direction // can be byte
  )
{
  // save wall location as front,left,right and back
  int wall_locs[3]; // can be byte

  if (left_sensor)
  {
    wall_locs[1] = 1;
    if (direction == 0) {
      // exeeding left wall
      if (x_position - 1 > 0)
        maze[x_position - 1][y_position] = 1;
    }
    else if (direction == 1) {
      // exeeding top wall
      if (y_position + 1 < 26)
         maze[x_position][y_position + 1] = 1;
    }
    else if (direction == 2) {
      // exeeding right wall
      if (x_position + 1 < 26)
        maze[x_position + 1][y_position] = 1;
    }
    else {
      // exeeding bottom wall
      if (y_position - 1 > 0)
        maze[x_position][y_position - 1] = 1;
    }
  }

  if (right_sensor)
  {
    wall_locs[2] = 1;
    if (direction == 0) {
      // exeeding right wall
      if (x_position + 1 < 26)
        maze[x_position + 1][y_position] = 1;
    }
    else if (direction == 1) {
      // exeeding bottom wall
      if (y_position - 1 > 0)
        maze[x_position][y_position - 1] = 1;
    }
    else if (direction == 2) {
      // exeeding left wall
      if (x_position - 1 > 0)
        maze[x_position - 1][y_position] = 1;
    }
    else {
      // exeeding top wall
      if (y_position + 1 < 26)
        maze[x_position][y_position + 1] = 1;
    }
  }

  if (front_sensor)
  {
    wall_locs[0] = 1;
    if (direction == 0) {
      // exeeding top wall
      if (y_position + 1 < 26)
        maze[x_position][y_position + 1] = 1;
    }
    else if (direction == 1) {
      // exeeding right wall
      if (x_position + 1 < 26)
        maze[x_position + 1][y_position] = 1;
    }
    else if (direction == 2) {
      // exeeding bottom wall
      if (y_position - 1 > 0)
        maze[x_position][y_position - 1] = 1;
    }
    else {
      // exeeding left wall
      if (y_position - 1 > 0)
        maze[x_position - 1][y_position] = 1;
    }
  }

  return wall_locs;

}

// one move using last mapping
int move(
  int** maze, 
  int* x_position, 
  int* y_position, 
  int direction,
  int* wall_locs,
  int* move_index,
  int* moves
) 
{

  int direction_values[4]; // cell values for direction cells
  int nxt_move;

  // checking available directions
  //north
  if (((*y_position)+1 < 26) && maze[(*x_position)][(*y_position)+1] != 1) {
    direction_values[0] = maze[(*x_position)][(*y_position)+2];
  }
  // east
  if (((*x_position)+1 < 26) && maze[(*x_position)+1][(*y_position)] != 1) {
    direction_values[1] = maze[(*x_position)+2][(*y_position)];
  }
  // south
  if (((*y_position)-1 > 0) && maze[(*x_position)][(*y_position)-1] != 1) {
    direction_values[2] = maze[(*x_position)][(*y_position)-2];
  }
  // west
  if (((*x_position)-1 > 0) && maze[(*x_position)-1][(*y_position)] != 1) {
    direction_values[3] = maze[(*x_position)-2][(*y_position)];
  }

  // only backward
  if (wall_locs[0] == 1 && wall_locs[1] == 1 && wall_locs[2] == 1) {
    // this is a dead end
    // no going back
    maze[(*x_position)][(*y_position)] = 10000; // big number
    // reset last move to restrict the box
    (*move_index) -= 2;
    nxt_move = 3;
  }
  // priority -> front > left > right > back
  // score priority -> go to min score
  else {
    if (wall_locs[0] == 0 && wall_locs[1] == 0) {
      int front = direction_values[direction];
      int left = direction_values[(direction+3)%4];
      nxt_move = front <= left ? 0 : 1;
      maze[(*x_position)][(*y_position)] = (*move_index) + 1;
    }
    else if (wall_locs[0] == 0 && wall_locs[2] == 0) {
      int front = direction_values[direction];
      int right = direction_values[(direction+1)%4];
      nxt_move = front <= right ? 0 : 2;
      maze[(*x_position)][(*y_position)] = (*move_index) + 1;
    }
    else if (wall_locs[1] == 0 && wall_locs[2] == 0) {
      int left = direction_values[direction+3];
      int right = direction_values[(direction+1)%4];
      nxt_move = left <= right ? 1 : 2;
      maze[(*x_position)][(*y_position)] = (*move_index) + 1;
    }
    else if (wall_locs[0] == 0 && wall_locs[1] == 0 && wall_locs[2] == 0){
      int front = direction_values[direction];
      int left = direction_values[direction+3];
      int right = direction_values[(direction+1)%4];
      nxt_move = (front <= left && front <= right) ? 0 : left <= right ? left : right;
      maze[(*x_position)][(*y_position)] = (*move_index) + 1;
    }
    // no wall front
    else if (wall_locs[0] == 0) {
      nxt_move = 0;
      maze[(*x_position)][(*y_position)] = (*move_index) + 1;
    }
    else if (wall_locs[1] == 0) {
      nxt_move = 1;
      maze[(*x_position)][(*y_position)] = (*move_index) + 1;
    }
    else if (wall_locs[2] == 0) {
      nxt_move = 2;
      maze[(*x_position)][(*y_position)] = (*move_index) + 1;
    }
  }

  moves[(*move_index)] = nxt_move;
  (*move_index)++;
  return nxt_move;

}


void loop()
{

  // get sensors
  bool left_sensor = 1;
  bool right_sensor = 1;
  bool front_sensor = 0;

  // assuming bottom left is start
  int x_start_position = 0;
  int y_start_position = 0;
  // 0 - north
  // 1 - east
  // 2 - south
  // 3 - west
  int direction = 0; // can be byte

  // 0 - forward
  // 1 - left
  // 2 - right
  // 3 - backward
  int moves[1000]; // can be byte
  int move_index = 0;

  int maze[27][27]; // can be byte

  while (true) // need to update condition
  {

    int x_position = x_start_position;
    int y_position = y_start_position;

    int* wall_locs = map_maze(maze, x_position, y_position, left_sensor, right_sensor, 
      front_sensor, direction);
    int nxt_move = move(maze, &x_position, &y_position, direction, wall_locs, &move_index, moves);

    // move with nxt_move value
    // front - 0; left - 1; right - 2

  }

}