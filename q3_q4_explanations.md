# Question 3 & 4 Deliverable Explanations

---

## Question 3 Explanation — Deadlock Prevention via Resource Ordering

Deadlock requires four simultaneous conditions: Mutual Exclusion, Hold and Wait, No Preemption, and Circular Wait. 

Resource Ordering eliminates the **Circular Wait** condition by assigning a global hierarchy to all locks (e.g., $\text{Lock1} < \text{Lock2}$) and requiring every process to acquire locks strictly in increasing rank order. Because no process can request a lower-ranked lock while holding a higher-ranked lock, a closed waiting cycle cannot form, making deadlocks impossible.

---

## Question 4 Explanation — Combined Synchronization & Deadlock Avoidance Strategy

### Strategy Used: Global Resource Ordering with Capacity Checking
- **Resource Hierarchy**: $\text{Printer (0)} < \text{Scanner (1)} < \text{Disk (2)}$
- **Capacity Bounds**: Atomic tracking of available instances per resource pool ($\text{Printer}=2, \text{Scanner}=1, \text{Disk}=2$).

### Why It Prevents Deadlock:
By enforcing strict ascending resource acquisition order, circular wait between different resource pools is prevented. By checking available instance counts before allocation, processes wait safely without over-allocating resources, allowing all 5 processes to complete their work across multiple iterations without hanging.
