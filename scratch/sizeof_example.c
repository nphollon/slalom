struct QueueNode {
  int data;
  struct QueueNode* next;
};

struct Queue {
  int length;
  struct QueueNode* first;
  struct QueueNode* last;
};

int main() {
  int x = sizeof(int);
  int y = sizeof(struct QueueNode);
  int z = sizeof(struct Queue);
  return x + y + z;
}
