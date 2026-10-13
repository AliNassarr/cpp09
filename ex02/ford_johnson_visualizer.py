"""
=============================================================================
 Ford-Johnson Algorithm (PmergeMe) - Manim Visualizer (Clean Layout Edition)
=============================================================================
Author: Expert CS & Animation Assistant
Usage:
    manim -pql ford_johnson_visualizer.py FordJohnsonVisualizer    # Low quality (preview)
    manim -pqh ford_johnson_visualizer.py FordJohnsonVisualizer    # High quality (1080p 60fps)

Key Layout Improvements:
    - Fixed 2-row grid system (Upper row: Winners/MainChain, Lower row: Losers).
    - Ample horizontal spacing (0.5+ unit margin between cards, no sticky nodes).
    - Labels placed strictly on dedicated label rows to prevent overlap collisions.
    - Perfect vertical alignment between paired winners and losers.
    - Zero LaTeX dependency (pure Pango/Cairo Text objects).

C++ INTEGRATION HOOK:
    Search for "C++ LOG / JSON HOOK" below to see how to replace the hardcoded
    array with dynamic trace logs exported from your C++ program.
=============================================================================
"""

from manim import *

# ---------------------------------------------------------------------------
# COLOR PALETTE (Modern Clean Flat UI)
# ---------------------------------------------------------------------------
COLOR_BG          = "#0f172a"  # Slate dark background
COLOR_DEFAULT     = "#38bdf8"  # Light sky blue
COLOR_WINNER      = "#22c55e"  # Vivid Emerald Green
COLOR_LOSER       = "#ef4444"  # Vibrant Coral Red
COLOR_STRAGGLER   = "#f97316"  # Bright Amber Orange
COLOR_MAIN_CHAIN  = "#6366f1"  # Indigo / Royal Blue
COLOR_SEARCH_BOX  = "#c084fc"  # Bright Purple for binary search bounds
COLOR_HIGHLIGHT   = "#facc15"  # Gold / Yellow for active focus
COLOR_TEXT_MUTED  = "#94a3b8"  # Muted slate text
COLOR_CONNECTOR   = "#475569"  # Slate connector line


class NumberNode(VGroup):
    """
    Visual card/node representing an integer element.
    Sized at 1.0 x 1.0 with rounded corners and centered bold text.
    """
    def __init__(self, value, color=COLOR_DEFAULT, width=1.0, height=1.0, **kwargs):
        super().__init__(**kwargs)
        self.value = value
        self.rect = RoundedRectangle(
            corner_radius=0.18,
            width=width,
            height=height,
            color=color,
            fill_color=color,
            fill_opacity=0.22,
            stroke_width=3.0
        )
        self.label = Text(str(value), font_size=28, weight=BOLD, color=WHITE)
        self.label.move_to(self.rect.get_center())
        self.add(self.rect, self.label)

    def set_node_color(self, new_color, fill_opacity=0.28):
        """Helper to tint both border and fill."""
        return [
            self.rect.animate.set_color(new_color).set_fill(new_color, opacity=fill_opacity)
        ]


class FordJohnsonVisualizer(Scene):
    def construct(self):
        self.camera.background_color = COLOR_BG

        # ===================================================================
        # C++ LOG / JSON HOOK:
        # To dynamically visualize arrays from your C++ program:
        #   import json, sys
        #   data = json.load(open("trace.json")) if len(sys.argv) > 1 else [9, 2, 12, 5, 7]
        # ===================================================================
        raw_data = [9, 2, 12, 5, 7]

        # -------------------------------------------------------------------
        # FIXED VERTICAL GRID CONSTANTS (Prevents all overlapping)
        # -------------------------------------------------------------------
        Y_TITLE        =  3.35
        Y_SUBTITLE     =  2.95
        Y_BADGE        =  2.35
        Y_TOP_ROW      =  1.10   # Upper row: Winners & mainChain
        Y_BOT_ROW      = -0.60   # Lower row: Pending Losers
        Y_CARD_INFO    = -1.80   # Comparison / Search indicator
        Y_EXPLANATION  = -3.10   # Bottom explanation card

        # -------------------------------------------------------------------
        # PERSISTENT HEADER UI
        # -------------------------------------------------------------------
        header_title = Text("Ford-Johnson Merge-Insertion Sort", font_size=28, weight=BOLD, color=WHITE)
        header_title.move_to([0, Y_TITLE, 0])

        sub_title = Text("Module 09 - ex02 (PmergeMe Visualizer)", font_size=15, color=COLOR_TEXT_MUTED)
        sub_title.move_to([0, Y_SUBTITLE, 0])

        self.play(FadeIn(header_title, shift=DOWN * 0.2), FadeIn(sub_title, shift=DOWN * 0.2))

        # Bottom Explanation Banner
        explanation_card = RoundedRectangle(
            corner_radius=0.15,
            width=12.6,
            height=1.0,
            color=COLOR_TEXT_MUTED,
            fill_color="#1e293b",
            fill_opacity=0.75,
            stroke_width=1.5
        ).move_to([0, Y_EXPLANATION, 0])

        explanation_text = Text(
            "Initial input: 5 elements [9, 2, 12, 5, 7]",
            font_size=17,
            color=WHITE
        ).move_to(explanation_card.get_center())

        self.play(Create(explanation_card), FadeIn(explanation_text))

        def update_explanation(new_text_str, text_color=WHITE):
            new_text = Text(new_text_str, font_size=17, color=text_color)
            new_text.move_to(explanation_card.get_center())
            return Transform(explanation_text, new_text)

        # -------------------------------------------------------------------
        # PHASE 1: PAIRING & STRAGGLER IDENTIFICATION
        # -------------------------------------------------------------------
        phase_badge = self.make_badge("PHASE 1: PAIRING & STRAGGLER", COLOR_DEFAULT)
        phase_badge.move_to([0, Y_BADGE, 0])
        self.play(FadeIn(phase_badge, shift=DOWN * 0.2))

        # Create Initial Nodes with generous horizontal spacing (1.6 per slot)
        nodes = [NumberNode(val) for val in raw_data]
        n9, n2, n12, n5, n7 = nodes

        # Slot layout for 5 cards in initial row (width=1.0, gap=0.5 -> spacing=1.5)
        initial_x = [-3.0, -1.5, 0.0, 1.5, 3.0]
        for i, node in enumerate(nodes):
            node.move_to([initial_x[i], Y_TOP_ROW, 0])

        self.play(
            LaggedStart(*[FadeIn(n, shift=UP * 0.3) for n in nodes], lag_ratio=0.12),
            update_explanation("Group adjacent elements into pairs: (9, 2) and (12, 5). Element 7 is the Straggler.")
        )
        self.wait(0.6)

        # Separate into 3 distinct visual columns:
        # Col 1 (Pair 1): x = -2.8
        # Col 2 (Pair 2): x =  0.2
        # Col 3 (Straggler): x = 3.2
        X_COL1 = -2.8
        X_COL2 =  0.2
        X_COL3 =  3.2

        # Shift to pair layout: 9 & 2 grouped, 12 & 5 grouped, 7 pushed to right
        self.play(
            n9.animate.move_to([X_COL1 - 0.65, Y_TOP_ROW, 0]),
            n2.animate.move_to([X_COL1 + 0.65, Y_TOP_ROW, 0]),
            n12.animate.move_to([X_COL2 - 0.65, Y_TOP_ROW, 0]),
            n5.animate.move_to([X_COL2 + 0.65, Y_TOP_ROW, 0]),
            n7.animate.move_to([X_COL3, Y_TOP_ROW, 0]),
            run_time=1.0
        )

        # Sub-labels for pairs below the top row
        pair1_label = Text("Pair 1", font_size=15, color=COLOR_TEXT_MUTED).move_to([X_COL1, Y_TOP_ROW - 0.9, 0])
        pair2_label = Text("Pair 2", font_size=15, color=COLOR_TEXT_MUTED).move_to([X_COL2, Y_TOP_ROW - 0.9, 0])
        tag_straggler = Text("Straggler (Odd)", font_size=15, color=COLOR_STRAGGLER, weight=BOLD).move_to([X_COL3, Y_TOP_ROW - 0.9, 0])

        self.play(
            FadeIn(pair1_label), FadeIn(pair2_label), FadeIn(tag_straggler),
            *n7.set_node_color(COLOR_STRAGGLER),
            update_explanation("Compare inside each pair: Winner (Larger, Green) goes UP, Loser (Smaller, Red) goes DOWN.")
        )
        self.wait(0.8)

        # Animate in-pair comparison signs with ample space
        comp_sign1 = Text(">", font_size=24, color=YELLOW).move_to([X_COL1, Y_TOP_ROW, 0])
        comp_sign2 = Text(">", font_size=24, color=YELLOW).move_to([X_COL2, Y_TOP_ROW, 0])

        self.play(FadeIn(comp_sign1), FadeIn(comp_sign2))
        self.wait(0.6)

        # Winners move to Upper Row, Losers move directly beneath them to Lower Row!
        # Col 1: Winner 9 at (X_COL1, Y_TOP_ROW), Loser 2 at (X_COL1, Y_BOT_ROW)
        # Col 2: Winner 12 at (X_COL2, Y_TOP_ROW), Loser 5 at (X_COL2, Y_BOT_ROW)
        # Straggler 7 centered vertically at (X_COL3, (Y_TOP_ROW + Y_BOT_ROW)/2)
        pos_w9  = np.array([X_COL1, Y_TOP_ROW, 0])
        pos_l2  = np.array([X_COL1, Y_BOT_ROW, 0])
        pos_w12 = np.array([X_COL2, Y_TOP_ROW, 0])
        pos_l5  = np.array([X_COL2, Y_BOT_ROW, 0])
        pos_s7  = np.array([X_COL3, (Y_TOP_ROW + Y_BOT_ROW) / 2, 0])

        # Vertical connector lines linking each winner to its paired loser
        conn_9_2  = DashedLine(pos_w9 + DOWN * 0.55, pos_l2 + UP * 0.55, color=COLOR_CONNECTOR, stroke_width=2.5)
        conn_12_5 = DashedLine(pos_w12 + DOWN * 0.55, pos_l5 + UP * 0.55, color=COLOR_CONNECTOR, stroke_width=2.5)

        self.play(
            FadeOut(comp_sign1), FadeOut(comp_sign2),
            FadeOut(pair1_label), FadeOut(pair2_label),
            n9.animate.move_to(pos_w9),
            n2.animate.move_to(pos_l2),
            n12.animate.move_to(pos_w12),
            n5.animate.move_to(pos_l5),
            n7.animate.move_to(pos_s7),
            tag_straggler.animate.move_to([X_COL3, pos_s7[1] - 0.85, 0]),
            *n9.set_node_color(COLOR_WINNER),
            *n12.set_node_color(COLOR_WINNER),
            *n2.set_node_color(COLOR_LOSER),
            *n5.set_node_color(COLOR_LOSER),
            Create(conn_9_2), Create(conn_12_5),
            run_time=1.3
        )

        # Clear Row Badges pinned to the far left (x = -5.4), never colliding with cards!
        lbl_winners_row = Text("Winners Row", font_size=15, color=COLOR_WINNER, weight=BOLD).move_to([-5.2, Y_TOP_ROW, 0])
        lbl_losers_row  = Text("Losers Row", font_size=15, color=COLOR_LOSER, weight=BOLD).move_to([-5.2, Y_BOT_ROW, 0])

        self.play(FadeIn(lbl_winners_row), FadeIn(lbl_losers_row))
        self.wait(1.0)

        # -------------------------------------------------------------------
        # PHASE 2: RECURSIVELY SORT WINNERS
        # -------------------------------------------------------------------
        new_badge_2 = self.make_badge("PHASE 2: SORT WINNERS & ALIGN PAIRS", COLOR_WINNER)
        new_badge_2.move_to(phase_badge.get_center())

        lbl_mainchain_row = Text("mainChain", font_size=15, color=COLOR_MAIN_CHAIN, weight=BOLD).move_to([-5.2, Y_TOP_ROW, 0])
        lbl_pending_row   = Text("Pending Losers", font_size=15, color=COLOR_LOSER, weight=BOLD).move_to([-5.2, Y_BOT_ROW, 0])

        self.play(
            Transform(phase_badge, new_badge_2),
            Transform(lbl_winners_row, lbl_mainchain_row),
            Transform(lbl_losers_row, lbl_pending_row),
            update_explanation("Step 2: Winners [9, 12] are sorted recursively into mainChain. Each loser stays paired below.")
        )
        self.wait(1.0)

        # -------------------------------------------------------------------
        # PHASE 3: FREE INSERTION OF FIRST LOSER (losers[0] = 2)
        # -------------------------------------------------------------------
        new_badge_3 = self.make_badge("PHASE 3: FREE INSERTION (0 COMPARISONS)", COLOR_HIGHLIGHT)
        new_badge_3.move_to(phase_badge.get_center())

        self.play(
            Transform(phase_badge, new_badge_3),
            update_explanation("Step 3: Loser 2 is smaller than its winner 9 (the smallest winner). Guaranteed smallest! 0 comparisons.")
        )

        # Highlight loser 2
        pulse_box = SurroundingRectangle(n2, color=COLOR_HIGHLIGHT, buff=0.1, corner_radius=0.15, stroke_width=3)
        self.play(Create(pulse_box), run_time=0.6)
        self.play(FadeOut(pulse_box), FadeOut(conn_9_2))

        # Re-center mainChain with 3 elements: [2, 9, 12]
        # X positions: -1.8, 0.0, 1.8 (spacing = 1.8, leaves 0.8 gap between cards)
        mc_x3 = [-1.8, 0.0, 1.8]
        pos_3_2  = np.array([mc_x3[0], Y_TOP_ROW, 0])
        pos_3_9  = np.array([mc_x3[1], Y_TOP_ROW, 0])
        pos_3_12 = np.array([mc_x3[2], Y_TOP_ROW, 0])
        pos_3_l5 = np.array([mc_x3[2], Y_BOT_ROW, 0])  # Stays directly under 12

        self.play(
            n2.animate.move_to(pos_3_2),
            n9.animate.move_to(pos_3_9),
            n12.animate.move_to(pos_3_12),
            n5.animate.move_to(pos_3_l5),
            conn_12_5.animate.put_start_and_end_on(pos_3_12 + DOWN * 0.55, pos_3_l5 + UP * 0.55),
            *n2.set_node_color(COLOR_MAIN_CHAIN),
            *n9.set_node_color(COLOR_MAIN_CHAIN),
            *n12.set_node_color(COLOR_MAIN_CHAIN),
            run_time=1.3
        )

        free_badge = Text("Free Insertion (+0 comps)", font_size=14, color=COLOR_HIGHLIGHT).move_to([mc_x3[0], Y_TOP_ROW + 0.85, 0])
        self.play(FadeIn(free_badge))
        self.wait(1.0)
        self.play(FadeOut(free_badge))

        # -------------------------------------------------------------------
        # PHASE 4: BOUNDED BINARY INSERTION (Loser 5)
        # -------------------------------------------------------------------
        new_badge_4 = self.make_badge("PHASE 4: BOUNDED BINARY SEARCH (JACOBSTHAL)", COLOR_SEARCH_BOX)
        new_badge_4.move_to(phase_badge.get_center())

        self.play(
            Transform(phase_badge, new_badge_4),
            FadeOut(conn_12_5),
            FadeOut(lbl_losers_row),
            update_explanation("Step 4a: Insert Loser 5. Since 5 was paired with 12, search space is BOUNDED to elements before 12!")
        )

        # Draw Search Boundary Box over [2, 9] (excludes 12!)
        search_box_1 = SurroundingRectangle(
            VGroup(n2, n9),
            color=COLOR_SEARCH_BOX,
            corner_radius=0.18,
            buff=0.18,
            stroke_width=2.5
        )
        bound_tag_1 = Text("Search Range: [2, 9] (Stops before paired winner 12!)", font_size=14, color=COLOR_SEARCH_BOX)
        bound_tag_1.move_to([search_box_1.get_center()[0], Y_TOP_ROW + 0.9, 0])

        self.play(
            Create(search_box_1), FadeIn(bound_tag_1),
            *n5.set_node_color(COLOR_HIGHLIGHT)
        )
        self.wait(0.8)

        # Show comparison steps clearly in the dedicated info row (Y_CARD_INFO)
        info_comp_5 = Text("Compare: 5 > 2 and 5 < 9  →  Insert between 2 and 9", font_size=16, color=YELLOW)
        info_comp_5.move_to([0, Y_CARD_INFO, 0])
        self.play(FadeIn(info_comp_5))
        self.wait(1.0)

        # Expand mainChain to 4 elements: [2, 5, 9, 12]
        # X positions: -2.7, -0.9, 0.9, 2.7 (spacing = 1.8)
        mc_x4 = [-2.7, -0.9, 0.9, 2.7]
        pos_4_2  = np.array([mc_x4[0], Y_TOP_ROW, 0])
        pos_4_5  = np.array([mc_x4[1], Y_TOP_ROW, 0])
        pos_4_9  = np.array([mc_x4[2], Y_TOP_ROW, 0])
        pos_4_12 = np.array([mc_x4[3], Y_TOP_ROW, 0])

        self.play(
            FadeOut(search_box_1), FadeOut(bound_tag_1), FadeOut(info_comp_5),
            n2.animate.move_to(pos_4_2),
            n5.animate.move_to(pos_4_5),
            n9.animate.move_to(pos_4_9),
            n12.animate.move_to(pos_4_12),
            *n5.set_node_color(COLOR_MAIN_CHAIN),
            run_time=1.3
        )
        self.wait(0.8)

        # -------------------------------------------------------------------
        # PHASE 4 (Part B): STRAGGLER INSERTION (7)
        # -------------------------------------------------------------------
        self.play(
            update_explanation("Step 4b: Insert Straggler 7. Has no paired winner → search bound is the ENTIRE chain!")
        )

        search_box_all = SurroundingRectangle(
            VGroup(n2, n5, n9, n12),
            color=COLOR_SEARCH_BOX,
            corner_radius=0.18,
            buff=0.18,
            stroke_width=2.5
        )
        bound_tag_all = Text("Search Range: Full Chain [2, 5, 9, 12]", font_size=14, color=COLOR_SEARCH_BOX)
        bound_tag_all.move_to([0, Y_TOP_ROW + 0.9, 0])

        self.play(
            Create(search_box_all), FadeIn(bound_tag_all),
            FadeOut(tag_straggler),
            *n7.set_node_color(COLOR_HIGHLIGHT)
        )
        self.wait(0.8)

        info_comp_7 = Text("Compare: 7 > 5 and 7 < 9  →  Insert between 5 and 9", font_size=16, color=YELLOW)
        info_comp_7.move_to([0, Y_CARD_INFO, 0])
        self.play(FadeIn(info_comp_7))
        self.wait(1.0)

        # Final Sorted Array with 5 elements: [2, 5, 7, 9, 12]
        # X positions: -3.2, -1.6, 0.0, 1.6, 3.2 (spacing = 1.6, leaves 0.6 gap between cards)
        mc_x5 = [-3.2, -1.6, 0.0, 1.6, 3.2]
        pos_5_2  = np.array([mc_x5[0], Y_TOP_ROW, 0])
        pos_5_5  = np.array([mc_x5[1], Y_TOP_ROW, 0])
        pos_5_7  = np.array([mc_x5[2], Y_TOP_ROW, 0])
        pos_5_9  = np.array([mc_x5[3], Y_TOP_ROW, 0])
        pos_5_12 = np.array([mc_x5[4], Y_TOP_ROW, 0])

        self.play(
            FadeOut(search_box_all), FadeOut(bound_tag_all), FadeOut(info_comp_7),
            n2.animate.move_to(pos_5_2),
            n5.animate.move_to(pos_5_5),
            n7.animate.move_to(pos_5_7),
            n9.animate.move_to(pos_5_9),
            n12.animate.move_to(pos_5_12),
            *n7.set_node_color(COLOR_MAIN_CHAIN),
            run_time=1.3
        )

        # -------------------------------------------------------------------
        # FINAL CELEBRATION
        # -------------------------------------------------------------------
        final_badge = self.make_badge("SORT COMPLETE: [2, 5, 7, 9, 12]", COLOR_WINNER)
        final_badge.move_to(phase_badge.get_center())

        self.play(
            Transform(phase_badge, final_badge),
            *[n.animate.set_fill(COLOR_WINNER, opacity=0.35).set_stroke(COLOR_WINNER, width=3.5) for n in [n2, n5, n7, n9, n12]],
            update_explanation("Ford-Johnson Complete! Optimal comparison bound strictly respected.", COLOR_WINNER)
        )

        # Large green checkmark icon
        checkmark = Text("✓", font_size=42, color=COLOR_WINNER, weight=BOLD)
        checkmark.move_to([mc_x5[4] + 1.3, Y_TOP_ROW, 0])
        self.play(FadeIn(checkmark, scale=1.3))

        self.wait(2.5)

    def make_badge(self, text_str, color):
        """Creates a modern rounded badge for phase titles."""
        label = Text(text_str, font_size=15, weight=BOLD, color=WHITE)
        pill = RoundedRectangle(
            corner_radius=0.15,
            width=label.width + 0.65,
            height=0.48,
            color=color,
            fill_color=color,
            fill_opacity=0.3,
            stroke_width=2.0
        )
        group = VGroup(pill, label)
        label.move_to(pill.get_center())
        return group
