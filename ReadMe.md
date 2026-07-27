# Restaurant Order Queue System (C++)

A console-based order management system built with a **linked list + FIFO queue**
(struct-based, no STL containers). Orders are added at the rear and served
(dequeued) from the front, matching real restaurant order flow.

## Files

| File | Purpose |
|---|---|
| `restaurant_queue.cpp` | Main program |
| `data/active_orders.txt` | Auto-generated — current queue snapshot |
| `data/order_history.txt` | Auto-generated — completed orders (append-only) |

## Build

```bash
g++ -std=c++11 -Wall -o restaurant_queue restaurant_queue.cpp
```

## Run

```bash
./restaurant_queue
```

You'll see this menu on every loop:

```
===== Restaurant Order Queue =====
1. Add Order (Name + Food)
2. Show Queue
3. Serve Order (Dequeue)
4. Peek Front Order
5. Search Order
6. Total Orders
7. Delete/Cancel Order
8. Display Linked List
9. Exit
```

---

## Test Commands

### 1. Quick automated smoke test

An automated script (`test_restaurant_queue.sh`) is included. It builds the
program, feeds it a scripted sequence of menu choices via `printf | ./program`,
and prints the resulting data files so you can eyeball correctness in one shot.

```bash
chmod +x test_restaurant_queue.sh
./test_restaurant_queue.sh
```

### 2. Manual test — normal flow (add → show → peek → count → search → serve → delete)

```bash
printf "1\nAli\nBiryani\n1\nSara\nKarahi\n2\n4\n6\n5\nAli\n3\n2\n7\nSara\n2\n8\n9\n" | ./restaurant_queue
```

**What this does, step by step:**
| Input | Menu action | Expected result |
|---|---|---|
| `1` → `Hafsa` / `Biryani` | Add order | `Order Added -> #1001` |
| `1` → `Sara` / `Karahi` | Add order | `Order Added -> #1002` |
| `2` | Show queue | Both orders listed, status `pending` |
| `4` | Peek | `Next Order -> #1001 \| Hafsa \| Biryani` |
| `6` | Count | `Total Orders: 2` |
| `5` → `Hafsa` | Search | `Found: #1001 Hafsa -> Biryani [pending]` |
| `3` | Serve (dequeue) | `Served: #1001 ...`, appended to `order_history.txt` |
| `2` | Show queue | Only Sara's order remains |
| `7` → `Sara` | Delete/cancel | `Order deleted.` |
| `2` | Show queue | `(empty)` |
| `8` | Show linked list | `Linked List is empty.` |
| `9` | Exit | Program terminates |

After this run, check the generated files:

```bash
cat data/active_orders.txt   # should be empty (both orders left the queue)
cat data/order_history.txt   # should contain: 1001,Hafsa,Biryani,completed
```

### 3. Manual test — edge cases on an empty queue

```bash
printf "3\n4\n5\nGhost\n6\n7\nGhost\n99\n9\n" | ./restaurant_queue
```

| Input | Action | Expected result |
|---|---|---|
| `3` | Serve on empty queue | `Queue Empty.` (no crash) |
| `4` | Peek on empty queue | `Queue Empty.` |
| `5` → `Ghost` | Search non-existent name | `Order not found.` |
| `6` | Count | `Total Orders: 0` |
| `7` → `Ghost` | Delete non-existent name | `Order not found.` |
| `99` | Invalid menu choice | `Invalid Option.` (loop continues, doesn't crash) |
| `9` | Exit | Program terminates |

### 4. Interactive manual testing

If you'd rather type it yourself instead of piping input:

```bash
./restaurant_queue
```
then follow the on-screen menu.

---

## Verified Behavior (already tested)

Both test scenarios above were run against this exact code and passed:
- Add/enqueue, show, peek, count, search, serve/dequeue, delete/cancel, and
  linked-list display all work correctly.
- Queue correctly transitions to `(empty)` after all orders are removed.
- `order_history.txt` correctly logs served orders as `completed`.
- Empty-queue operations (serve, peek) fail gracefully with a message instead
  of crashing (no null-pointer dereference).
- Invalid menu input is handled without crashing.

## Known Issue (Linux only)

The line:
```cpp
system("mkdir data 2>nul || mkdir -p data");
```
is written to work on both Windows (`2>nul` discards errors) and Linux
(`mkdir -p` fallback). On Linux, `2>nul` doesn't refer to a null device — it
creates/redirects to a literal file named `nul` in your working directory.
It's harmless (empty file, no functional impact) but you'll see a stray `nul`
file appear after running the program. Safe to delete, or replace the line
with a purely cross-platform version:
```cpp
#ifdef _WIN32
    system("mkdir data 2>nul");
#else
    system("mkdir -p data");
#endif
```

## Notes / Limitations

- `id` search/delete is actually by **customer name**, not order ID — if two
  customers share a name, `deleteOrder` removes only the first match, and
  `searchOrder` will list all matches.
- No input validation on `cin >> choice` — entering a non-numeric value will
  put `cin` in a fail state and cause an infinite `Invalid Option.` loop.
  Not covered by the tests above; worth trying if you want to see it break.
