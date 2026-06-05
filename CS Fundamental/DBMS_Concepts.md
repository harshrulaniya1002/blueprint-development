# DBMS Concepts - Complete Interview Guide

---

# Table of Contents

1. [Introduction to DBMS](#1-introduction-to-dbms)
2. [ER Model](#2-er-model)
3. [Relational Model & Relational Algebra](#3-relational-model--relational-algebra)
4. [SQL](#4-sql---structured-query-language)
5. [Normalization](#5-normalization)
6. [Transactions & ACID](#6-transactions--acid-properties)
7. [Concurrency Control](#7-concurrency-control)
8. [Indexing](#8-indexing)
9. [Query Processing & Optimization](#9-query-processing--optimization)
10. [Recovery System](#10-recovery-system)
11. [Real-World Case Study: Amazon](#11-real-world-case-study-amazon-e-commerce)
12. [Interview Questions](#12-important-interview-questions)

---

# 1. Introduction to DBMS

## What is a DBMS?

A **Database Management System** is software that stores, retrieves, and manages data efficiently. It provides an interface between the database and users/applications.

## File System vs DBMS

| Feature | File System | DBMS |
|---------|-------------|------|
| **Redundancy** | High (duplicate files) | Low (normalization) |
| **Consistency** | Hard to maintain | Enforced by constraints |
| **Concurrent Access** | No support | Transactions & locking |
| **Security** | OS-level only | Fine-grained (row/column) |
| **Recovery** | Manual | Automatic (WAL, logs) |
| **Query Language** | None (write code) | SQL |

## Types of DBMS

| Type | Model | Example |
|------|-------|---------|
| Relational (RDBMS) | Tables | MySQL, PostgreSQL, Oracle |
| Document (NoSQL) | JSON documents | MongoDB, CouchDB |
| Key-Value | Key → Value | Redis, DynamoDB |
| Column-family | Column families | Cassandra, HBase |
| Graph | Nodes + Edges | Neo4j |

## Three-Schema Architecture

```
External Level  → User views (View 1, View 2, ...)
      ↕  Logical Data Independence
Conceptual Level → Logical schema (tables, relationships)
      ↕  Physical Data Independence
Internal Level  → Physical storage (files, indexes, blocks)
```

- **Logical Data Independence**: Change schema without changing views.
- **Physical Data Independence**: Change storage without changing schema.

## DBMS Languages

| Language | Purpose | Commands |
|----------|---------|----------|
| **DDL** | Define schema | CREATE, ALTER, DROP |
| **DML** | Query/modify data | SELECT, INSERT, UPDATE, DELETE |
| **DCL** | Access control | GRANT, REVOKE |
| **TCL** | Transactions | COMMIT, ROLLBACK, SAVEPOINT |

---

# 2. ER Model

The **ER Model** is a high-level conceptual model to design databases before implementation.

## Components

| Component | Symbol | Example |
|-----------|--------|---------|
| Entity | Rectangle | Student, Course |
| Key Attribute | Underlined ellipse | roll_no |
| Multivalued Attr | Double ellipse | phone_numbers |
| Derived Attr | Dashed ellipse | age (from DOB) |
| Composite Attr | Sub-ellipses | name → (first, last) |
| Relationship | Diamond | "enrolls_in" |
| Weak Entity | Double rectangle | Dependent (of Employee) |

## Cardinality

- **1:1** — Person : Passport
- **1:N** — Department : Employees
- **M:N** — Students : Courses

## Real-World ER Example: University

**Entities**: Student, Course, Professor, Department

**Relationships**:
- Student *enrolls_in* Course (M:N)
- Professor *teaches* Course (1:N)
- Department *has* Professor (1:N)
- Student *belongs_to* Department (M:1)

**Attributes**:
- Student: roll_no (PK), name(first, last), DOB, age (derived), phone_nos (multivalued)
- Course: course_id (PK), title, credits
- Weak Entity: Dependent (of Professor) — PK = emp_id + dep_name

## ER to Relational Mapping Rules

| ER Construct | Rule |
|-------------|------|
| Strong Entity | Table with PK = key attribute |
| Weak Entity | PK = partial key + owner's PK (FK) |
| 1:1 Relationship | FK on total participation side |
| 1:N Relationship | FK on the N-side |
| M:N Relationship | New junction table with FKs from both |
| Multivalued Attr | Separate table (entity PK, value) |
| Composite Attr | Flatten into simple attributes |
| Derived Attr | Don't store; compute in queries |

## ER to SQL Example

```sql
CREATE TABLE Department (
    dept_id   INT PRIMARY KEY,
    dept_name VARCHAR(50)
);

CREATE TABLE Professor (
    emp_id  INT PRIMARY KEY,
    name    VARCHAR(100),
    salary  DECIMAL(10,2),
    dept_id INT REFERENCES Department(dept_id)  -- 1:N
);

CREATE TABLE Student (
    roll_no    INT PRIMARY KEY,
    first_name VARCHAR(50),
    last_name  VARCHAR(50),
    dob        DATE,
    dept_id    INT REFERENCES Department(dept_id)  -- M:1
);

CREATE TABLE Course (
    course_id INT PRIMARY KEY,
    title     VARCHAR(100),
    credits   INT,
    prof_id   INT REFERENCES Professor(emp_id)  -- 1:N
);

-- M:N junction table
CREATE TABLE Enrollment (
    roll_no   INT REFERENCES Student(roll_no),
    course_id INT REFERENCES Course(course_id),
    grade     CHAR(2),
    PRIMARY KEY (roll_no, course_id)
);

-- Weak entity
CREATE TABLE Dependent (
    emp_id   INT REFERENCES Professor(emp_id),
    dep_name VARCHAR(50),
    relation VARCHAR(20),
    PRIMARY KEY (emp_id, dep_name)
);
```

## Generalization, Specialization, Aggregation

| Concept | Direction | Example |
|---------|-----------|---------|
| Generalization | Bottom-up: combine sub-entities | Car + Truck → Vehicle |
| Specialization | Top-down: divide super-entity | Person → Student + Employee |
| Aggregation | Treat relationship as entity | "Enrollment" participates in "Feedback" |

---

# 3. Relational Model & Relational Algebra

## Keys

| Key Type | Definition | Example |
|----------|-----------|---------|
| **Super Key** | Any set uniquely identifying a tuple | {roll_no}, {roll_no, name} |
| **Candidate Key** | Minimal super key | {roll_no}, {email} |
| **Primary Key** | Chosen candidate key | roll_no |
| **Alternate Key** | Candidate keys not chosen as PK | email |
| **Foreign Key** | References PK of another table | Student.dept_id → Dept.dept_id |
| **Composite Key** | PK with multiple attributes | (roll_no, course_id) |

## Integrity Constraints

1. **Domain**: Values from valid domain (age > 0)
2. **Key**: No duplicate key values
3. **Entity Integrity**: PK cannot be NULL
4. **Referential Integrity**: FK must match PK or be NULL

## Relational Algebra

| Operator | Symbol | Description |
|----------|--------|-------------|
| Selection | σ_condition(R) | Filter rows |
| Projection | π_cols(R) | Select columns |
| Union | R ∪ S | All tuples in R or S |
| Intersection | R ∩ S | Tuples in both |
| Difference | R - S | Tuples in R not in S |
| Cartesian Product | R × S | All combinations |
| Natural Join | R ⋈ S | Join on common attributes |
| Division | R ÷ S | Tuples in R associated with ALL of S |

### Examples

```
Find CS students:
  π_name(σ_dept='CS'(Student))

Find students in "DBMS":
  π_name(Student ⋈ Enrollment ⋈ σ_title='DBMS'(Course))

Students enrolled in ALL courses (division):
  π_roll,cid(Enrollment) ÷ π_cid(Course)
```

---

# 4. SQL — Structured Query Language

## DDL

```sql
CREATE TABLE Employee (
    emp_id     INT PRIMARY KEY AUTO_INCREMENT,
    name       VARCHAR(100) NOT NULL,
    email      VARCHAR(100) UNIQUE,
    salary     DECIMAL(10,2) DEFAULT 50000,
    dept_id    INT REFERENCES Department(dept_id)
        ON DELETE SET NULL ON UPDATE CASCADE,
    CHECK (salary > 0)
);

ALTER TABLE Employee ADD COLUMN phone VARCHAR(15);
ALTER TABLE Employee DROP COLUMN phone;

DROP TABLE Employee;      -- Removes table + data + schema
TRUNCATE TABLE Employee;  -- Removes all rows, keeps schema
```

### DROP vs TRUNCATE vs DELETE

| | DROP | TRUNCATE | DELETE |
|---|------|----------|--------|
| Type | DDL | DDL | DML |
| Removes | Table+data+schema | All rows | Specific rows |
| WHERE? | No | No | Yes |
| Rollback? | No | No | Yes |
| Triggers? | No | No | Yes |
| Speed | Fast | Fast | Slow (row-by-row) |

## DML

```sql
INSERT INTO Employee (name, salary, dept_id) VALUES
    ('Harsh', 90000, 1),
    ('Amit', 75000, 2),
    ('Priya', 85000, 1);

UPDATE Employee SET salary = salary * 1.10 WHERE dept_id = 1;
DELETE FROM Employee WHERE emp_id = 5;
```

## SELECT — Real-World E-commerce Queries

Tables: `Customer(cust_id, name, city)`, `Product(prod_id, name, price, category)`, `Orders(order_id, cust_id, order_date, total)`, `OrderItem(order_id, prod_id, quantity, price)`

### Q1: Mumbai customers who spent > 50K
```sql
SELECT c.name, SUM(o.total) AS total_spent
FROM Customer c
JOIN Orders o ON c.cust_id = o.cust_id
WHERE c.city = 'Mumbai'
GROUP BY c.cust_id, c.name
HAVING SUM(o.total) > 50000
ORDER BY total_spent DESC;
```

### Q2: Second highest price
```sql
-- Method 1: Subquery
SELECT MAX(price) FROM Product
WHERE price < (SELECT MAX(price) FROM Product);

-- Method 2: LIMIT/OFFSET
SELECT DISTINCT price FROM Product ORDER BY price DESC LIMIT 1 OFFSET 1;

-- Method 3: Window Function
SELECT name, price FROM (
    SELECT name, price,
        DENSE_RANK() OVER (ORDER BY price DESC) AS rnk
    FROM Product
) t WHERE rnk = 2;
```

### Q3: Customers who bought EVERY Electronics product (Division in SQL)
```sql
SELECT c.name
FROM Customer c
WHERE NOT EXISTS (
    SELECT prod_id FROM Product WHERE category = 'Electronics'
    EXCEPT
    SELECT oi.prod_id FROM Orders o
    JOIN OrderItem oi ON o.order_id = oi.order_id
    WHERE o.cust_id = c.cust_id
);
```

### Q4: Running total per customer
```sql
SELECT cust_id, order_date, total,
    SUM(total) OVER (PARTITION BY cust_id ORDER BY order_date) AS running_total
FROM Orders;
```

### Q5: Top 3 selling products
```sql
SELECT p.name, SUM(oi.quantity) AS total_sold
FROM Product p
JOIN OrderItem oi ON p.prod_id = oi.prod_id
GROUP BY p.prod_id, p.name
ORDER BY total_sold DESC
LIMIT 3;
```

## Joins

| Type | Result |
|------|--------|
| **INNER JOIN** | Only matching rows |
| **LEFT JOIN** | All left + matching right (NULL if no match) |
| **RIGHT JOIN** | All right + matching left |
| **FULL OUTER JOIN** | All from both (NULL where no match) |
| **CROSS JOIN** | Cartesian product |
| **SELF JOIN** | Table joined with itself |

### Join Example with Data

```
Employee:                    Department:
emp_id | name  | dept_id    dept_id | dept_name
1      | Harsh | 10         10      | Engineering
2      | Amit  | 20         20      | Marketing
3      | Priya | NULL       30      | Finance

INNER JOIN:  (Harsh, Engineering), (Amit, Marketing)
LEFT JOIN:   + (Priya, NULL)
FULL OUTER:  + (NULL, Finance)
```

## Subqueries

| Type | Description |
|------|-------------|
| **Scalar** | Returns single value: `WHERE sal > (SELECT AVG(sal)...)` |
| **Row** | Returns single row |
| **Table** | Returns table: `FROM (SELECT ...) AS t` |
| **Correlated** | References outer query: executes once per outer row |

### Correlated Subquery Example
```sql
-- Employees earning above their department average
SELECT name, salary, dept_id
FROM Employee e1
WHERE salary > (
    SELECT AVG(salary) FROM Employee e2
    WHERE e2.dept_id = e1.dept_id
);
```

## Views, Triggers, Stored Procedures

```sql
-- VIEW: Virtual table
CREATE VIEW HighEarners AS
SELECT name, salary FROM Employee WHERE salary > 100000;

-- TRIGGER: Auto-execute on data changes
CREATE TRIGGER salary_audit
AFTER UPDATE ON Employee
FOR EACH ROW
BEGIN
    INSERT INTO AuditLog (emp_id, old_salary, new_salary, changed_at)
    VALUES (OLD.emp_id, OLD.salary, NEW.salary, NOW());
END;

-- STORED PROCEDURE
CREATE PROCEDURE GiveRaise(IN dept INT, IN pct DECIMAL)
BEGIN
    UPDATE Employee SET salary = salary * (1 + pct/100) WHERE dept_id = dept;
END;
CALL GiveRaise(10, 15);  -- 15% raise to dept 10
```

---

# 5. Normalization

## What is Normalization?

Process of organizing a database to reduce **redundancy** and prevent **anomalies** (insertion, deletion, update). Decomposes tables following **normal forms**.

## Anomalies (Why We Normalize)

Table: `StudentCourse(roll, name, dept, course, prof)`

| roll | name | dept | course | prof |
|------|------|------|--------|------|
| 1 | Harsh | CS | DBMS | Dr. Sharma |
| 1 | Harsh | CS | OS | Dr. Gupta |
| 2 | Amit | EC | DBMS | Dr. Sharma |

- **Update Anomaly**: Change Harsh's dept → must update multiple rows.
- **Insertion Anomaly**: Can't add a department without a student.
- **Deletion Anomaly**: If Amit drops DBMS, we lose info that Dr. Sharma teaches DBMS.

## Functional Dependencies (FD)

**X → Y** means: same X value ⇒ same Y value. "X determines Y."

**Armstrong's Axioms:**
1. **Reflexivity**: Y ⊆ X ⇒ X → Y
2. **Augmentation**: X → Y ⇒ XZ → YZ
3. **Transitivity**: X → Y, Y → Z ⇒ X → Z

**Derived**: Union (X→Y, X→Z ⇒ X→YZ), Decomposition (X→YZ ⇒ X→Y, X→Z)

## Finding Candidate Keys (Closure)

```
R(A, B, C, D, E), FDs: {A→B, BC→D, D→E, E→A}

Find AC⁺:
  AC⁺ = {A,C}
  A→B:    AC⁺ = {A,B,C}
  BC→D:   AC⁺ = {A,B,C,D}
  D→E:    AC⁺ = {A,B,C,D,E} = R  ✓

AC is a candidate key.

Shortcut: C never appears on RHS → C must be in every candidate key.
All candidate keys: {AC, BC, DC, EC}
```

## Normal Forms

### 1NF — Atomic values only

No multivalued attributes, no repeating groups.

**Violation**: phones = "9876, 9123" → Fix: separate rows or table.

### 2NF — No partial dependencies

Non-prime attribute must not depend on a *subset* of candidate key.

```
Enrollment(roll, course_id, student_name, grade)
  roll → student_name (partial dep on PK {roll, course_id})

Fix: Student(roll, student_name) + Enrollment(roll, course_id, grade)
```

### 3NF — No transitive dependencies

For every FD X → A: either X is superkey OR A is prime attribute.

```
Employee(emp_id, dept_id, dept_name)
  emp_id → dept_id → dept_name (transitive)

Fix: Employee(emp_id, dept_id) + Department(dept_id, dept_name)
```

### BCNF — Every determinant is a superkey

Stricter than 3NF: no exception for prime attributes.

```
Teaching(student, course, professor)
FDs: {student,course}→professor, professor→course

professor is NOT a superkey → violates BCNF

Fix: ProfCourse(professor, course) + StudentProf(student, professor)
Warning: May NOT preserve dependency {student,course}→professor
```

## Normal Forms Summary

| NF | Rule | Eliminates |
|----|------|------------|
| **1NF** | Atomic values | Repeating groups |
| **2NF** | No partial deps | Partial key redundancy |
| **3NF** | No transitive deps | Non-key redundancy |
| **BCNF** | Determinant = superkey | All FD redundancy |
| **4NF** | No multi-valued deps | Independent MVDs |

**Hierarchy**: 1NF ⊂ 2NF ⊂ 3NF ⊂ BCNF ⊂ 4NF ⊂ 5NF

Most systems aim for **3NF or BCNF**. Sometimes **denormalize** for read performance.

## Lossless Join & Dependency Preservation

Decomposition of R into R1, R2 is **lossless** iff:
- R1 ∩ R2 → R1 OR R1 ∩ R2 → R2

**3NF decomposition**: always lossless + dependency preserving.
**BCNF decomposition**: always lossless, may NOT preserve dependencies.

---

# 6. Transactions & ACID Properties

## ACID

| Property | Meaning | Real-World Example |
|----------|---------|-------------------|
| **Atomicity** | All or nothing | Bank transfer: debit+credit both or neither |
| **Consistency** | Valid state → valid state | Total money unchanged after transfer |
| **Isolation** | No interference between concurrent txns | Two bookings for last seat: one wins |
| **Durability** | Committed = permanent | Power failure after commit → data safe |

## Bank Transfer Example

```sql
BEGIN TRANSACTION;
UPDATE Account SET balance = balance - 10000 WHERE acc_id = 'A';
-- If system crashes here: Atomicity ensures rollback
UPDATE Account SET balance = balance + 10000 WHERE acc_id = 'B';
COMMIT;
-- Durability: changes permanent even if crash after this
```

## Transaction States

```
Active → Partially Committed → Committed
  ↓              ↓
Failed ────→ Aborted
```

## Schedules & Serializability

Two operations **conflict** if: different transactions + same data item + at least one is write.

**Conflict pairs**: RW, WR, WW.

### Conflict Serializability Test

Build **precedence graph**:
- For each conflict pair, draw edge from earlier transaction to later.
- **No cycle** = conflict serializable ✓
- **Cycle** = NOT conflict serializable ✗

### Example

```
T1: R(A) W(A) R(B) W(B)
T2: R(A) W(A) R(B) W(B)

If T1.R(A) before T2.W(A): edge T1→T2
If T2.R(A) before T1.W(A): edge T2→T1
Both edges → CYCLE → NOT conflict serializable
```

## Recoverability Hierarchy

```
Serial ⊂ Strict ⊂ Cascadeless ⊂ Recoverable ⊂ All Schedules
```

- **Recoverable**: T2 reads from T1 → T1 commits before T2.
- **Cascadeless**: Only read committed data.
- **Strict**: Don't touch item until writer commits/aborts.

---

# 7. Concurrency Control

## Problems

| Problem | Description |
|---------|-------------|
| **Dirty Read** | Read uncommitted data |
| **Non-repeatable Read** | Same query gives different results |
| **Phantom Read** | New rows appear in repeated query |
| **Lost Update** | Two writes, one is lost |

## Isolation Levels

| Level | Dirty Read | Non-repeatable | Phantom |
|-------|-----------|----------------|---------|
| Read Uncommitted | Yes | Yes | Yes |
| Read Committed | **No** | Yes | Yes |
| Repeatable Read | No | **No** | Yes |
| Serializable | No | No | **No** |

## Lock-Based Protocols

**Lock types**: Shared (S) — for reading, Exclusive (X) — for writing.

**Compatibility**: S-S ✓, S-X ✗, X-S ✗, X-X ✗

### Two-Phase Locking (2PL)

1. **Growing Phase**: Acquire locks, never release.
2. **Shrinking Phase**: Release locks, never acquire.

**Guarantees**: Conflict serializability.
**Problem**: Can cause deadlocks.

**Variants**:
| Variant | Rule | Benefit |
|---------|------|---------|
| Basic 2PL | Grow then shrink | Serializable |
| Strict 2PL | Hold X-locks until commit | No cascading rollback |
| Rigorous 2PL | Hold ALL locks until commit | Strictest, simplest |

### Deadlock in DBMS

```
T1: Lock-X(A), then tries Lock-X(B) → waits
T2: Lock-X(B), then tries Lock-X(A) → waits
→ DEADLOCK!
```

**Solutions**:
- **Wait-Die**: Older waits, younger dies (rollback).
- **Wound-Wait**: Older wounds (forces rollback of younger), younger waits.
- **Timeout**: Rollback if waiting too long.
- **Detection**: Wait-for graph; cycle → rollback victim.

## Timestamp-Based Protocol

Each transaction gets timestamp at start. Operations ordered by timestamp. **Deadlock-free** but may cause cascading rollbacks.

## MVCC (Multi-Version Concurrency Control)

Maintain **multiple versions** of data. Readers see a **snapshot** at their start time. **Readers never block writers, writers never block readers.**

Used by: PostgreSQL, MySQL (InnoDB), Oracle.

---

# 8. Indexing

## Why Indexing?

Without index: full table scan O(n). With index: O(log n) or O(1).

## Index Types

| Type | On | Example |
|------|-----|---------|
| **Primary** | Ordering key of sorted file | Index on emp_id |
| **Clustering** | Non-key ordering field | Index on dept_id |
| **Secondary** | Non-ordering field | Index on name |
| **Dense** | Entry for every record | Fast lookup, more space |
| **Sparse** | Entry per block | Less space, slower |

## B-Tree

Properties (order m):
- Every node has at most m children.
- Non-leaf (except root) has at least ⌈m/2⌉ children.
- All leaves at same level.
- Node with k children has k-1 keys.

## B+ Tree (Most Important for DBMS)

**Key differences from B-Tree:**
- **All data in leaf nodes only**. Internal nodes = routing keys.
- **Leaf nodes linked** in a linked list → efficient range queries.
- Internal nodes hold more keys → shallower tree.

```
B+ Tree:
        [30 | 60]              ← Internal (routing)
       /    |     \
  [10|20] → [30|40|50] → [60|70|80]   ← Leaves (data + linked)
```

### B+ Tree Calculation Example

```
Block size = 4096 bytes, Key = 8 bytes, Pointer = 8 bytes

Internal order (p):
  p × 8 + (p-1) × 8 ≤ 4096
  16p - 8 ≤ 4096 → p = 256 (255 keys per node)

For 1,000,000 records:
  Level 0 (root): 1 node
  Level 1: up to 256 nodes
  Level 2 (leaves): up to 256² = 65,536 nodes → covers 16.7M records

Height ≈ 3. Only 3 disk reads to find any record among 1 million!
```

## Hashing vs B+ Tree

| | Hashing | B+ Tree |
|---|---------|---------|
| Exact match | O(1) — faster | O(log n) |
| Range queries | Not supported | Excellent |
| Ordered access | No | Yes |

---

# 9. Query Processing & Optimization

## Steps

```
SQL Query → Parser (syntax check) → Relational Algebra tree → Optimizer → Execution → Result
```

## Join Algorithms

| Algorithm | Cost | When |
|-----------|------|------|
| Nested Loop | O(n×m) | Small tables |
| Sort-Merge | O(n log n + m log m) | Sortable data |
| Hash Join | O(n+m) | Equality joins, enough memory |
| Index Nested Loop | O(n × log m) | Index on join column |

## Optimization Heuristics

1. **Push selections down** — filter early to reduce intermediate results.
2. **Push projections down** — remove unnecessary columns early.
3. **Use indexes** — avoid full table scans.
4. **Choose best join order** — order massively affects cost.
5. **Avoid Cartesian products** — only join with conditions.

---

# 10. Recovery System

## Write-Ahead Log (WAL)

Every operation logged **before** modifying DB.

Format: `<Ti, X, old_value, new_value>`

**Rules:**
1. Log record written BEFORE the DB modification.
2. All log records of a transaction written BEFORE commit.

## Recovery Operations

| Operation | When | How |
|-----------|------|-----|
| **Undo** | Transaction didn't commit | Restore old values from log |
| **Redo** | Transaction committed but not flushed | Apply new values from log |

## Checkpointing

Marks a point where all committed changes are on disk. Recovery only processes after last checkpoint.

### Recovery Example

```
Log:
1. <T1, start>
2. <T1, A, 100, 200>
3. <T2, start>
4. CHECKPOINT
5. <T2, B, 50, 80>
6. <T1, commit>
7. <T3, start>
8. <T3, C, 30, 60>
9. --- CRASH ---

Recovery:
  T1: Committed → REDO (A = 200)
  T2: Not committed → UNDO (B = 50)
  T3: Not committed → UNDO (C = 30)
```

---

# 11. Real-World Case Study: Amazon E-Commerce

## Step 1: Database Design

**Entities**: Customer, Product, Order, OrderItem, Seller, Review, Payment

**Relationships**:
- Customer *places* Order (1:N)
- Order *contains* Products (M:N via OrderItem)
- Seller *lists* Products (1:N)
- Customer *writes* Review for Product (M:N)
- Order *has* Payment (1:1)

After **3NF normalization**: 7 tables, each fact stored once.

## Step 2: Unnormalized → Normalized

**Bad design:**

| order_id | cust_name | cust_email | prod_name | prod_price | qty | seller_name |
|----------|-----------|------------|-----------|------------|-----|-------------|
| 1001 | Harsh | h@e.com | iPhone | 79999 | 1 | Apple Store |
| 1001 | Harsh | h@e.com | Case | 999 | 2 | MobiShop |
| 1002 | Amit | a@e.com | iPhone | 79999 | 1 | Apple Store |

**Problems**: Harsh's email repeated, iPhone info repeated, can't add product without order.

**After 3NF**: Customer, Product, Seller, Order, OrderItem, Payment, Review — each fact once.

## Step 3: SQL in Action

```sql
-- Customer searches for "laptop"
SELECT prod_id, title, price, AVG(r.rating) AS avg_rating
FROM Product p
LEFT JOIN Review r ON p.prod_id = r.prod_id
WHERE p.title LIKE '%laptop%' AND p.price BETWEEN 30000 AND 80000
GROUP BY p.prod_id, p.title, p.price
HAVING AVG(r.rating) >= 4.0
ORDER BY avg_rating DESC, p.price ASC;
```

## Step 4: Transaction — Payment

```sql
BEGIN TRANSACTION;

SELECT quantity FROM Inventory WHERE prod_id = 101 FOR UPDATE;
-- FOR UPDATE: acquires X-lock (2PL growing phase)

UPDATE Inventory SET quantity = quantity - 1 WHERE prod_id = 101;
INSERT INTO Orders (cust_id, order_date, total, status)
    VALUES (5001, NOW(), 79999, 'PENDING');
INSERT INTO Payment (order_id, method, amount, status)
    VALUES (LAST_INSERT_ID(), 'UPI', 79999, 'SUCCESS');
UPDATE Orders SET status = 'CONFIRMED' WHERE order_id = LAST_INSERT_ID();

COMMIT;  -- All locks released (2PL shrinking phase)
-- If payment fails: ROLLBACK (atomicity restores stock)
```

## Step 5: Concurrency — Flash Sale

10,000 users try to buy last 5 iPhones:

```sql
-- WITHOUT concurrency control:
-- User A reads: quantity = 5
-- User B reads: quantity = 5
-- User A: UPDATE SET quantity = 4
-- User B: UPDATE SET quantity = 4  ← LOST UPDATE! Should be 3

-- WITH 2PL:
-- User A: SELECT ... FOR UPDATE (X-lock on row)
-- User B: SELECT ... FOR UPDATE (BLOCKS, waits)
-- User A: UPDATE, COMMIT (releases lock)
-- User B: Now reads quantity = 4, proceeds correctly
```

**Amazon's approach:**
- Optimistic Locking (version column)
- Queue-based processing (FIFO)
- Redis atomic DECR for counter

## Step 6: Indexing — Why Search is Fast

```sql
CREATE INDEX idx_prod_title ON Product(title);
CREATE INDEX idx_prod_cat_price ON Product(category, price);
CREATE INDEX idx_order_cust ON Orders(cust_id);
CREATE INDEX idx_review_prod ON Review(prod_id);
```

| Query | Without Index | With B+ Tree |
|-------|--------------|-------------|
| Find by ID | Scan 10M rows | 3-4 disk reads |
| Find by category | Scan 10M rows | ~100 disk reads |
| Price range | Scan 10M rows | Leaf scan (fast) |

## Step 7: Recovery — Crash During Order

```
WAL: T start → Inventory write → Order insert → CRASH

T never committed → UNDO: restore inventory, delete order.
Customer sees: "Order failed, please retry."
Stock correctly restored. No data corruption.
```

## Step 8: Views & Security

```sql
-- Customer sees only their orders
CREATE VIEW CustomerDashboard AS
SELECT o.order_id, o.order_date, o.status, o.total, p.title
FROM Orders o JOIN OrderItem oi ON o.order_id = oi.order_id
JOIN Product p ON oi.prod_id = p.prod_id
WHERE o.cust_id = CURRENT_USER_ID();

-- Access control
GRANT SELECT ON CustomerDashboard TO customer_role;
REVOKE DELETE ON Product FROM seller_role;
```

---

# 12. Important Interview Questions

### Q1: What are ACID properties?
Atomicity (all-or-nothing), Consistency (valid states), Isolation (no interference), Durability (permanent after commit).

### Q2: DELETE vs TRUNCATE vs DROP?
DELETE: DML, row-by-row, rollback-able, fires triggers. TRUNCATE: DDL, removes all rows fast, no rollback. DROP: removes entire table + schema.

### Q3: What is normalization?
Decomposing tables to reduce redundancy. 1NF→2NF→3NF→BCNF. Each eliminates a specific type of dependency issue.

### Q4: 3NF vs BCNF?
3NF allows X→A if A is prime. BCNF requires X to be superkey always. BCNF may not preserve dependencies.

### Q5: What is a B+ Tree?
Balanced tree, data only in leaves, leaves linked. Supports range queries. Height ~3 for millions of records.

### Q6: What is 2PL?
Growing (acquire locks) then shrinking (release). Ensures conflict serializability. Can deadlock.

### Q7: What is MVCC?
Multiple versions of data. Readers see snapshots. Readers don't block writers. Used by PostgreSQL, MySQL.

### Q8: Deadlock in DBMS?
T1 holds A wants B, T2 holds B wants A. Solutions: Wait-Die, Wound-Wait, timeout, detection (wait-for graph).

### Q9: Types of joins?
INNER (matching only), LEFT (all left), RIGHT (all right), FULL OUTER (all), CROSS (cartesian), SELF (with itself).

### Q10: What is a view?
Virtual table from a query. Doesn't store data. Provides security and simplicity.

### Q11: Primary Key vs Unique Key?
PK: no NULL, only one per table. Unique: allows one NULL, multiple per table. Both enforce uniqueness.

### Q12: Clustered vs Non-clustered index?
Clustered: reorders table data physically (one per table). Non-clustered: separate pointer structure (multiple per table).

### Q13: What is WAL?
Write-Ahead Logging. Log changes before writing to DB. Enables undo and redo after crash.

### Q14: Conflict serializability test?
Build precedence graph from conflicting operations. No cycle = serializable. Cycle = not serializable.

### Q15: What is denormalization?
Adding controlled redundancy for read performance. Used in data warehouses. Trade-off: faster reads, slower writes.

### Q16: Correlated subquery?
Inner query references outer query. Executes once per outer row. Example: employees above dept average.

### Q17: What is a foreign key?
Column referencing PK of another table. Enforces referential integrity. ON DELETE options: CASCADE, SET NULL, RESTRICT.

### Q18: What is sharding?
Horizontal partitioning across multiple servers. Each shard holds a subset of rows. Used for massive scalability.

### Q19: SQL vs NoSQL?
SQL: structured, ACID, joins, vertical scaling. NoSQL: flexible schema, BASE, horizontal scaling. SQL for transactions, NoSQL for big data.

### Q20: What is a trigger?
Auto-executed SQL on INSERT/UPDATE/DELETE events. Used for auditing, validation, cascading updates.

---

# Quick Reference Formulas

```
B+ Tree height ≈ ⌈log_p(n)⌉        (p = order, n = records)
Internal order p: p×P + (p-1)×K ≤ Block_size
Leaf order pL: pL×(K+RecPtr) + P ≤ Block_size

Lossless test: R1 ∩ R2 → R1 or R1 ∩ R2 → R2
Closure: X⁺ = all attributes determined by X
Candidate Key: X⁺ = R and no proper subset has this

NF Hierarchy: 1NF ⊂ 2NF ⊂ 3NF ⊂ BCNF ⊂ 4NF ⊂ 5NF
Schedule Hierarchy: Serial ⊂ Strict ⊂ Cascadeless ⊂ Recoverable
Serializability: Conflict Ser. ⊂ View Ser.
```

---

*Comprehensive DBMS guide for interview preparation. Good luck!*
