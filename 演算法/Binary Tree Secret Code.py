# 演算法分析機測
# 學號:11027104/11027133/11027149
# 姓名:侯如蓁/李若菱/游婕歆
# 中原大學資訊工程系
import time

class TreeNode:
    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None

#根據前序和中序的結果建二元樹。
#根據前序遍歷的第一個節點（根節點）和中序遍歷中根節點的位置來切分左右子樹
#然後再建左右子樹。
def build_tree(preorder, inorder):
    if not preorder:
        return None
    root_val = preorder[0]
    root = TreeNode(root_val)
    idx = inorder.index(root_val)
    root.left = build_tree(preorder[1:idx + 1], inorder[:idx])  # 左子樹構建
    root.right = build_tree(preorder[idx + 1:], inorder[idx + 1:])  # 右子樹構建
    return root

#後序
def postorder_traversal(root, result):
    if root:
        postorder_traversal(root.left, result)  # 後序遍歷左子樹
        postorder_traversal(root.right, result)  # 後序遍歷右子樹
        result.append(root.val)  # 將節點值加入結果列表

def decode_message(preorder_str, inorder_str):
    preorder = list(preorder_str)
    inorder = list(inorder_str)
    root = build_tree(preorder, inorder)  # 建二元樹
    result = []
    postorder_traversal(root, result)  # 後序遍歷二元樹並收集節點值
    return ''.join(result)  # 後序遍歷結果

if __name__ == "__main__":
    start_time = time.time()
    decoded_messages = []
    print("Input: ")
    while True:
        preorder_str = input().strip()
        if preorder_str == '0':
            break
        inorder_str = input().strip()
        decoded_messages.append(decode_message(preorder_str, inorder_str))

    print("\nOutput: ")
    for message in decoded_messages:
        print(message)
    total_time = time.time() - start_time
    print("\nTIME:", total_time)
