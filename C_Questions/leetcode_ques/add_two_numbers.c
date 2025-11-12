/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    int count = 0;
    int res = 0;
    int temp = 0;
    struct ListNode* tmp_l1 = l1;
    struct ListNode* tmp_l2 = l2;

    while(l1 != NULL){
        l1 = l1->next;
        count++;
    }
    
    l1 = tmp_l1;
    int arr[count];
    
    for(int i = 0; i < 3; i++){
        res = (l1->val + l2->val);
        if((res/10) != 0){
            temp = res - 10;
        }else{
            temp = 0;
        }
        arr[count - 1 - i] = res + temp;
        l1 = l1->next;
        l2 = l2->next;
    }
    l1 = tmp_l1;
    l2 = tmp_l2;
    int j = 0;
    while(l1 != NULL){

        l1->val = arr[j++];
        l1 = l1->next;
    }
    l1 = tmp_l1;
    return l1;
}
