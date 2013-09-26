
/*
 * nlog(n) for sorting and linear for rest
 *
 * arrays sort p by y order
 * arrays sort p by polar order
 *
 * hull.push(p[0])
 * hull.push(p[1])
 *
 * for (i = 2; i < n; i++) {
 *   top = hull.pop()
 *   while (ccw(hull.peek(), top, p[i]) <= 0) {
 *     top = hull.pop()
 *   }
 *   hull.push(top);
 *   hull.push(p[i]);
 * }
 */
