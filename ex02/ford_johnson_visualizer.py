"""
=============================================================================
 Ford-Johnson Algorithm (PmergeMe) - Manim Visualizer
=============================================================================
Author: Expert CS & Animation Assistant
Usage:
    manim -pql ford_johnson_visualizer.py FordJohnsonVisualizer    # Low quality (fast preview)
    manim -pqh ford_johnson_visualizer.py FordJohnsonVisualizer    # High quality (1080p 60fps)

Prerequisites:
    pip install manim

C++ INTEGRATION HOOK:
    Search for "C++ LOG / JSON HOOK" below to see how to replace the hardcoded
    array with dynamic trace logs emitted by your C++ PmergeMe program.
=============================================================================
"""

from manim import *

# ---------------------------------------------------------------------------
# COLOR PALETTE (Modern Flat / Material Theme)
# ---------------------------------------------------------------------------
COLOR_BG          = "#0f172a"  # Slate dark background
COLOR_DEFAULT     = "#38bdf8"  # Light sky blue
COLOR_WINNER      = "#22c55e"  # Vivid Emerald Green
COLOR_LOSER       = "#ef4444"  # Vibrant Coral Red
COLOR_STRAGGLER   = "#f97316"  # Bright Amber Orange
COLOR_MAIN_CHAIN  = "#6366f1"  # Indigo / Royal Blue
COLOR_SEARCH_BOX  = "#a855f7"  # Purple for binary search bounds
COLOR_HIGHLIGHT   = "#eab308"  # Gold / Yellow for active focus
COLOR_TEXT_MUTED  = "#94a3b8"  # Muted slate text


class NumberNode(VGroup):
    """
    Visual card/node representing an integer element with smooth color shifts.
    Uses pure Text Mobjects for maximum portability without LaTeX dependencies.
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
        # Optional: set background color
        self.camera.background_color = COLOR_BG

        # ===================================================================
        # C++ LOG / JSON HOOK:
        # To dynamically visualize arrays from your C++ program:
        #   import json, sys
        #   data = json.load(open("trace.json")) if len(sys.argv) > 1 else [9, 2, 12, 5, 7]
        # ===================================================================
        raw_data = [9, 2, 12, 5, 7]

        # -------------------------------------------------------------------
        # TOP HEADER & PERSISTENT UI
        # -------------------------------------------------------------------
        header_title = Text("Ford-Johnson Merge-Insertion Sort", font_size=30, weight=BOLD, color=WHITE)
        header_title.to_edge(UP, buff=0.35)

        sub_title = Text("Module 09 - ex02 (PmergeMe)", font_size=16, color=COLOR_TEXT_MUTED)
        sub_title.next_to(header_title, DOWN, buff=0.1)

        self.play(FadeIn(header_title, shift=DOWN * 0.2), FadeIn(sub_title, shift=DOWN * 0.2))

        # Bottom Explanation Callout Box
        explanation_card = RoundedRectangle(
            corner_radius=0.15,
            width=12.2,
            height=1.1,
            color=COLOR_TEXT_MUTED,
            fill_color="#1e293b",
            fill_opacity=0.7,
            stroke_width=1.5
        ).to_edge(DOWN, buff=0.4)

        explanation_text = Text(
            "Initializing array of 5 elements: [9, 2, 12, 5, 7]",
            font_size=18,
            color=WHITE
        ).move_to(explanation_card.get_center())

        self.play(Create(explanation_card), FadeIn(explanation_text))

        def update_explanation(new_text_str, text_color=WHITE):
            new_text = Text(new_text_str, font_size=18, color=text_color)
            new_text.move_to(explanation_card.get_center())
            return Transform(explanation_text, new_text)

        # -------------------------------------------------------------------
        # PHASE 1: PAIRING & STRAGGLER IDENTIFICATION
        # -------------------------------------------------------------------
        phase_badge = self.make_badge("PHASE 1: PAIRING & STRAGGLER", COLOR_DEFAULT)
        phase_badge.next_to(sub_title, DOWN, buff=0.25)
        self.play(FadeIn(phase_badge, shift=DOWN * 0.2))

        # Build Initial Nodes
        nodes = [NumberNode(val) for val in raw_data]
        node_group = VGroup(*nodes).arrange(RIGHT, buff=0.45).move_to(UP * 0.6)

        self.play(
            LaggedStart(*[FadeIn(n, shift=UP * 0.3) for n in nodes], lag_ratio=0.15),
            update_explanation("Step 1: Group adjacent numbers into pairs. Since size is odd, the last is a Straggler.")
        )
        self.wait(0.8)

        # Split into Pair 1, Pair 2, and Straggler
        n9, n2, n12, n5, n7 = nodes

        pair1 = VGroup(n9, n2)
        pair2 = VGroup(n12, n5)
        straggler = n7

        self.play(
            pair1.animate.shift(LEFT * 1.6),
            pair2.animate.shift(LEFT * 0.2),
            straggler.animate.shift(RIGHT * 1.8),
            run_time=1.2
        )

        # Labels for pairs
        brace_p1 = Brace(pair1, DOWN, color=COLOR_TEXT_MUTED, buff=0.15)
        label_p1 = brace_p1.get_text("Pair 1").scale(0.65)

        brace_p2 = Brace(pair2, DOWN, color=COLOR_TEXT_MUTED, buff=0.15)
        label_p2 = brace_p2.get_text("Pair 2").scale(0.65)

        tag_straggler = Text("Straggler", font_size=16, color=COLOR_STRAGGLER, weight=BOLD)
        tag_straggler.next_to(straggler, DOWN, buff=0.3)

        self.play(
            FadeIn(brace_p1), FadeIn(label_p1),
            FadeIn(brace_p2), FadeIn(label_p2),
            FadeIn(tag_straggler),
            *straggler.set_node_color(COLOR_STRAGGLER),
            update_explanation("Comparing pairs: larger becomes Winner (Green), smaller becomes Loser (Red).")
        )
        self.wait(0.8)

        # Animate in-pair comparisons
        # Pair 1: 9 > 2
        # Pair 2: 12 > 5
        comp_sign1 = Text(">", font_size=26, color=YELLOW).move_to((n9.get_center() + n2.get_center()) / 2)
        comp_sign2 = Text(">", font_size=26, color=YELLOW).move_to((n12.get_center() + n5.get_center()) / 2)

        self.play(FadeIn(comp_sign1), FadeIn(comp_sign2))
        self.wait(0.5)

        self.play(
            FadeOut(comp_sign1), FadeOut(comp_sign2),
            # Shift winners up, losers down
            n9.animate.shift(UP * 0.55),
            n12.animate.shift(UP * 0.55),
            n2.animate.shift(DOWN * 0.55),
            n5.animate.shift(DOWN * 0.55),
            *n9.set_node_color(COLOR_WINNER),
            *n12.set_node_color(COLOR_WINNER),
            *n2.set_node_color(COLOR_LOSER),
            *n5.set_node_color(COLOR_LOSER),
            run_time=1.2
        )

        # Add Winners & Losers row labels
        tag_winners = Text("Winners", font_size=16, color=COLOR_WINNER, weight=BOLD).next_to(n9, LEFT, buff=0.4)
        tag_losers = Text("Losers", font_size=16, color=COLOR_LOSER, weight=BOLD).next_to(n2, LEFT, buff=0.4)
        self.play(FadeIn(tag_winners), FadeIn(tag_losers))
        self.wait(1.0)

        # -------------------------------------------------------------------
        # PHASE 2: SORTING WINNERS & ALIGNING LOSERS
        # -------------------------------------------------------------------
        new_badge_2 = self.make_badge("PHASE 2: RECURSIVELY SORT WINNERS", COLOR_WINNER)
        new_badge_2.move_to(phase_badge.get_center())

        self.play(
            Transform(phase_badge, new_badge_2),
            FadeOut(brace_p1), FadeOut(label_p1),
            FadeOut(brace_p2), FadeOut(label_p2),
            FadeOut(tag_winners), FadeOut(tag_losers),
            update_explanation("Step 2: Winners [9, 12] are sorted recursively into mainChain. Losers stay aligned.")
        )
        self.wait(0.5)

        # Move Winners into mainChain row (y = 0.9)
        # Move Losers directly below their paired winners (y = -0.7)
        pos_w9 = np.array([-1.2, 0.9, 0])
        pos_w12 = np.array([1.2, 0.9, 0])
        pos_l2 = np.array([-1.2, -0.7, 0])
        pos_l5 = np.array([1.2, -0.7, 0])
        pos_straggler = np.array([3.4, 0.1, 0])

        arrow_9_2 = DashedLine(pos_w9 + DOWN * 0.5, pos_l2 + UP * 0.5, color=COLOR_TEXT_MUTED, stroke_width=2)
        arrow_12_5 = DashedLine(pos_w12 + DOWN * 0.5, pos_l5 + UP * 0.5, color=COLOR_TEXT_MUTED, stroke_width=2)

        lbl_mainchain = Text("mainChain (Sorted Winners)", font_size=17, color=COLOR_WINNER, weight=BOLD)
        lbl_mainchain.next_to(pos_w9, LEFT, buff=0.5)

        lbl_losers = Text("Pending Losers (paired)", font_size=17, color=COLOR_LOSER, weight=BOLD)
        lbl_losers.next_to(pos_l2, LEFT, buff=0.5)

        self.play(
            n9.animate.move_to(pos_w9),
            n12.animate.move_to(pos_w12),
            n2.animate.move_to(pos_l2),
            n5.animate.move_to(pos_l5),
            straggler.animate.move_to(pos_straggler),
            tag_straggler.animate.next_to(pos_straggler, DOWN, buff=0.25),
            FadeIn(arrow_9_2), FadeIn(arrow_12_5),
            FadeIn(lbl_mainchain), FadeIn(lbl_losers),
            run_time=1.3
        )
        self.wait(1.0)

        # -------------------------------------------------------------------
        # PHASE 3: FREE INSERTION (FIRST LOSER)
        # -------------------------------------------------------------------
        new_badge_3 = self.make_badge("PHASE 3: FREE INSERTION (0 COMPARISONS)", COLOR_HIGHLIGHT)
        new_badge_3.move_to(phase_badge.get_center())

        self.play(
            Transform(phase_badge, new_badge_3),
            update_explanation("Step 3: Loser 2 is smaller than Winner 9 (the smallest winner). Guaranteed smallest! 0 comparisons.")
        )

        # Highlight 2 and 9
        pulse_ring = Circle(radius=0.75, color=COLOR_HIGHLIGHT, stroke_width=3.5).move_to(n2.get_center())
        self.play(Create(pulse_ring), run_time=0.6)
        self.play(FadeOut(pulse_ring), FadeOut(arrow_9_2))

        # Animate 2 moving to front of mainChain
        # New mainChain positions: [2, 9, 12]
        mc_pos_2 = np.array([-2.4, 0.9, 0])
        mc_pos_9 = np.array([0.0, 0.9, 0])
        mc_pos_12 = np.array([2.4, 0.9, 0])

        self.play(
            n2.animate.move_to(mc_pos_2),
            n9.animate.move_to(mc_pos_9),
            n12.animate.move_to(mc_pos_12),
            # Keep loser 5 aligned under 12
            arrow_12_5.animate.put_start_and_end_on(mc_pos_12 + DOWN * 0.5, pos_l5 + RIGHT * 1.2 + UP * 0.5),
            n5.animate.move_to(pos_l5 + RIGHT * 1.2),
            *n2.set_node_color(COLOR_MAIN_CHAIN),
            *n9.set_node_color(COLOR_MAIN_CHAIN),
            *n12.set_node_color(COLOR_MAIN_CHAIN),
            run_time=1.4
        )

        badge_free = Text("Free Insertion (+0 comps)", font_size=15, color=COLOR_HIGHLIGHT)
        badge_free.next_to(n2, UP, buff=0.25)
        self.play(FadeIn(badge_free))
        self.wait(1.0)
        self.play(FadeOut(badge_free))

        # -------------------------------------------------------------------
        # PHASE 4: JACOBSTHAL & BOUNDED BINARY INSERTION
        # -------------------------------------------------------------------
        new_badge_4 = self.make_badge("PHASE 4: BOUNDED BINARY SEARCH (JACOBSTHAL)", COLOR_SEARCH_BOX)
        new_badge_4.move_to(phase_badge.get_center())

        self.play(
            Transform(phase_badge, new_badge_4),
            FadeOut(arrow_12_5),
            FadeOut(lbl_losers),
            update_explanation("Step 4a: Insert next loser 5. Paired with 12 → Search space BOUNDED to elements before 12!")
        )
        self.wait(0.5)

        # Highlight bound: search only before 12 (i.e. [2, 9])
        search_box_1 = SurroundingRectangle(
            VGroup(n2, n9),
            color=COLOR_SEARCH_BOX,
            corner_radius=0.15,
            buff=0.2,
            stroke_width=2.5
        )
        lbl_bound_1 = Text("Search Range: [2, 9] (Stops before 12!)", font_size=15, color=COLOR_SEARCH_BOX)
        lbl_bound_1.next_to(search_box_1, UP, buff=0.2)

        self.play(Create(search_box_1), FadeIn(lbl_bound_1), *n5.set_node_color(COLOR_HIGHLIGHT))
        self.wait(0.8)

        # Binary search visualization: compare 5 vs 2 -> 5 > 2, 5 vs 9 -> 5 < 9
        comp_text_5 = Text("5 > 2 and 5 < 9  →  Insert between 2 and 9", font_size=16, color=YELLOW)
        comp_text_5.next_to(search_box_1, DOWN, buff=0.25)
        self.play(FadeIn(comp_text_5))
        self.wait(1.0)

        # Re-arrange mainChain with 5 inserted: [2, 5, 9, 12]
        mc_4_pos_2  = np.array([-3.0, 0.9, 0])
        mc_4_pos_5  = np.array([-1.0, 0.9, 0])
        mc_4_pos_9  = np.array([1.0, 0.9, 0])
        mc_4_pos_12 = np.array([3.0, 0.9, 0])

        self.play(
            FadeOut(search_box_1), FadeOut(lbl_bound_1), FadeOut(comp_text_5),
            n2.animate.move_to(mc_4_pos_2),
            n5.animate.move_to(mc_4_pos_5),
            n9.animate.move_to(mc_4_pos_9),
            n12.animate.move_to(mc_4_pos_12),
            *n5.set_node_color(COLOR_MAIN_CHAIN),
            run_time=1.3
        )
        self.wait(0.8)

        # -------------------------------------------------------------------
        # PHASE 4 (Part B): STRAGGLER INSERTION
        # -------------------------------------------------------------------
        self.play(
            update_explanation("Step 4b: Insert Straggler 7. Has no paired winner → search bound is the ENTIRE chain!")
        )

        search_box_all = SurroundingRectangle(
            VGroup(n2, n5, n9, n12),
            color=COLOR_SEARCH_BOX,
            corner_radius=0.15,
            buff=0.2,
            stroke_width=2.5
        )
        lbl_bound_all = Text("Search Range: Full Chain [2, 5, 9, 12]", font_size=15, color=COLOR_SEARCH_BOX)
        lbl_bound_all.next_to(search_box_all, UP, buff=0.2)

        self.play(
            Create(search_box_all), FadeIn(lbl_bound_all),
            FadeOut(tag_straggler),
            *straggler.set_node_color(COLOR_HIGHLIGHT)
        )
        self.wait(0.8)

        comp_text_7 = Text("7 > 5 and 7 < 9  →  Insert between 5 and 9", font_size=16, color=YELLOW)
        comp_text_7.next_to(search_box_all, DOWN, buff=0.25)
        self.play(FadeIn(comp_text_7))
        self.wait(1.0)

        # Final Sorted Positions: [2, 5, 7, 9, 12]
        final_pos_2  = np.array([-3.6, 0.9, 0])
        final_pos_5  = np.array([-1.8, 0.9, 0])
        final_pos_7  = np.array([0.0, 0.9, 0])
        final_pos_9  = np.array([1.8, 0.9, 0])
        final_pos_12 = np.array([3.6, 0.9, 0])

        self.play(
            FadeOut(search_box_all), FadeOut(lbl_bound_all), FadeOut(comp_text_7),
            n2.animate.move_to(final_pos_2),
            n5.animate.move_to(final_pos_5),
            straggler.animate.move_to(final_pos_7),
            n9.animate.move_to(final_pos_9),
            n12.animate.move_to(final_pos_12),
            *straggler.set_node_color(COLOR_MAIN_CHAIN),
            run_time=1.4
        )

        # -------------------------------------------------------------------
        # COMPLETION & CELEBRATION
        # -------------------------------------------------------------------
        final_badge = self.make_badge("SORT COMPLETE: [2, 5, 7, 9, 12]", COLOR_WINNER)
        final_badge.move_to(phase_badge.get_center())

        self.play(
            Transform(phase_badge, final_badge),
            *[n.animate.set_fill(COLOR_WINNER, opacity=0.35).set_stroke(COLOR_WINNER, width=3.5) for n in [n2, n5, straggler, n9, n12]],
            update_explanation("Ford-Johnson Complete! Optimal comparisons achieved. Fully sorted array: [2, 5, 7, 9, 12]", COLOR_WINNER)
        )

        # Add checkmark icon / text
        checkmark = Text("✓", font_size=40, color=COLOR_WINNER, weight=BOLD)
        checkmark.next_to(final_pos_12, RIGHT, buff=0.5)
        self.play(FadeIn(checkmark, scale=1.3))

        self.wait(2.5)

    def make_badge(self, text_str, color):
        """Creates a modern rounded badge for phase titles."""
        label = Text(text_str, font_size=15, weight=BOLD, color=WHITE)
        pill = RoundedRectangle(
            corner_radius=0.15,
            width=label.width + 0.6,
            height=0.48,
            color=color,
            fill_color=color,
            fill_opacity=0.3,
            stroke_width=2.0
        )
        group = VGroup(pill, label)
        label.move_to(pill.get_center())
        return group


# ===========================================================================
# C++ / JSON TRACE ADAPTER EXAMPLE:
#
# If your C++ program outputs a JSON trace of every comparison and insert,
# you can subclass FordJohnsonVisualizer and load it like this:
#
# class DynamicFordJohnsonVisualizer(FordJohnsonVisualizer):
#     def get_initial_array(self):
#         import json
#         with open("pmergeme_trace.json", "r") as f:
#             data = json.load(f)
#             return data.get("initial_array", [9, 2, 12, 5, 7])
# ===========================================================================
