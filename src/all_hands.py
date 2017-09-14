

suits = ['s', 'h', 'd', 'c']
hands = ['2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A']

cards = []
for h in hands:
    for s in suits:
        cards.append(h+s)

card_len = len(cards)

for i in range(0, card_len):
    for j in range(i+1, card_len):
        print(cards[i]+","+cards[j])


