/*

    PROBLEM:
    Given a string containing only '(' and ')', return the length
    of the longest valid (well-formed) parentheses substring.

    Example 1: s = "(()"      -> Output: 2   ("()")
    Example 2: s = ")()())"   -> Output: 4   ("()()")
    Example 3: s = ""         -> Output: 0

    ============================================================
    WHY THE CONSTRAINTS MATTER
    ============================================================
    0 <= s.length <= 3 * 10^4
    s[i] is '(' or ')'

    - length can be 0        -> must handle empty string safely, answer is 0
    - length up to 30,000    -> brute force O(n^3) checking every substring
                                 would take way too long (~27 trillion ops),
                                 so the solution MUST run around O(n)
    - only '(' and ')'       -> no other characters to worry about

    This tells us upfront: think of a single-pass or near-linear solution,
    not something that checks every substring one by one.
*/
/*
    ============================================================
    APPROACH 1: STACK METHOD
    ============================================================

    THOUGHT PROCESS (step by step):

    Step 1 - What does "valid" mean?
        Every '(' must have a matching ')' after it, properly nested.
        "(())" is valid, ")(" is not.

    Step 2 - How do we naturally check balance?
        By hand, we cancel each ')' with the MOST RECENT unmatched '('.
        "Most recent first" is exactly how a stack behaves -> use a stack.

    Step 3 - But we need LENGTH, not just true/false.
        A normal validity check pushes/pops characters. Here we push
        INDICES instead, because subtracting two indices instantly
        gives us a length.

    Step 4 - What do we push, and when?
        - See '(' -> push its index (a bracket waiting for a partner)
        - See ')' -> pop the last unmatched '(' -> try to form a pair

    Step 5 - What if there's nothing to pop (stack becomes empty)?
        This ')' has no partner. It breaks everything before it,
        so we push its own index as a NEW starting line (new base).

    Step 6 - How do we measure length using only indices?
        The stack always holds the index just BEFORE the current
        valid run started. So (currentIndex - stack.top()) instantly
        gives the length of the valid run whenever a match happens.

    Step 7 - Why push -1 at the very start?
        If a valid run starts right at index 0, we still need something
        to subtract from. -1 acts as "the position just before the
        string begins" -> a safety base marker.

    Time: O(n)   Space: O(n) - stack can hold up to n indices worst case
*/
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestValidParentheses(string s) {
        stack<int> st;
        st.push(-1);                 // base marker
        int maxLen = 0;

        for (int i = 0; i < (int)s.size(); i++) {
            if (s[i] == '(') {
                st.push(i);           // pending open bracket
            } else {
                st.pop();              // try to match with last open bracket
                if (st.empty()) {
                    st.push(i);        // no match -> this becomes new base
                } else {
                    maxLen = max(maxLen, i - st.top());  // valid length found
                }
            }
        }
        return maxLen;
    }
};

int main() {
    string s;
    cout << "Enter a string of parentheses: ";
    cin >> s;
    
    int result = longestValidParentheses_Optimal(s);
    cout << "Longest valid parentheses length: " << result << endl;
    
    return 0;
}

/*Time: O(n)   Space: O(n) - stack can hold up to n indices worst case*/



/*
    ============================================================
    WHY MOVE TO A BETTER APPROACH?
    ============================================================
    The stack works correctly, but ask: "Can this be done with less memory?"

    For a string like "(((((", the stack would end up holding almost
    every index -> O(n) space just to track positions.

    If we only need COUNTS of open/close brackets (not their exact
    positions), we can avoid the stack entirely. That question leads
    directly to the next approach.
*/


/*
    ============================================================
    APPROACH 2: TWO PASS COUNTER METHOD (OPTIMAL)
    ============================================================

    THOUGHT PROCESS (step by step):

    Step 1 - Replace the stack with two counters.
        Scan left to right, counting '(' seen so far (openCount)
        and ')' seen so far (closeCount).

    Step 2 - When is something valid?
        If openCount == closeCount at any point, everything scanned
        since the last reset is perfectly balanced.
        Its length is (2 * closeCount).

    Step 3 - When does it break?
        If closeCount ever exceeds openCount, this ')' has no match.
        Nothing valid can be built across it -> reset both counters to 0.

    Step 4 - Does this catch every case?
        Test on "(()":  open stays ahead of close forever, so
        open == close never happens -> we MISS the valid "()" inside.
        Left-to-right alone misses a leftover unmatched '('.

    Step 5 - The fix: do it again, right to left.
        Same idea, but scanning backwards, with roles swapped ->
        reset happens when openCount > closeCount instead.
        This backward pass catches exactly what the forward pass missed.

    Step 6 - Result:
        Two integer counters, two passes, no stack, no array.
        This gives O(1) space - the most optimal solution possible.

    Time: O(n)   Space: O(1)
*/
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestValidParentheses(string s) {
        int n = s.size();              // store string length
        int maxLen = 0;                // final answer - longest valid length
        int open = 0, close = 0;       // counters for '(' and ')'
        
        // PASS 1: Left to Right - catches extra unmatched ')'
        for (int i = 0; i < n; i++) {
            if (s[i] == '(') open++;   // count opening bracket
            else close++;              // count closing bracket
            
            if (open == close) {       // balanced substring found
                maxLen = max(maxLen, 2 * close);  // update max length
            }
            else if (close > open) {   // extra ')' found - cannot be matched
                open = close = 0;      // reset counters, start fresh
            }
        }
        
        // PASS 2: Right to Left - catches extra unmatched '('
        open = close = 0;              // reset counters for second pass
        
        for (int i = n - 1; i >= 0; i--) {  // traverse from end
            if (s[i] == '(') open++;   // count opening bracket
            else close++;              // count closing bracket
            
            if (open == close) {       // balanced substring found
                maxLen = max(maxLen, 2 * open);  // update max length
            }
            else if (open > close) {   // extra '(' found - cannot be matched
                open = close = 0;      // reset counters, start fresh
            }
        }
        
        return maxLen;                 // return the longest valid length found
    }
};


int main() {
    string s;
    cout << "Enter a string of parentheses: ";
    cin >> s;
    
    int result = longestValidParentheses(s);
    cout << "Longest valid parentheses length: " << result << endl;
    
    return 0;
}