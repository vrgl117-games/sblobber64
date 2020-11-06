#!/usr/bin/env bash

#UI
convert -strip resources/gfx/sprites/logo.png -crop 32 resources/gfx/sprites_sets/logo-%d_0.png

# studio logo
convert -strip -brightness-contrast -10 resources/gfx/sprites/intro.png resources/gfx/sprites/intro_9.png
convert -strip -brightness-contrast -20 resources/gfx/sprites/intro.png resources/gfx/sprites/intro_8.png
convert -strip -brightness-contrast -30 resources/gfx/sprites/intro.png resources/gfx/sprites/intro_7.png
convert -strip -brightness-contrast -40 resources/gfx/sprites/intro.png resources/gfx/sprites/intro_6.png
convert -strip -brightness-contrast -50 resources/gfx/sprites/intro.png resources/gfx/sprites/intro_5.png
convert -strip -brightness-contrast -60 resources/gfx/sprites/intro.png resources/gfx/sprites/intro_4.png
convert -strip -brightness-contrast -70 resources/gfx/sprites/intro.png resources/gfx/sprites/intro_3.png
convert -strip -brightness-contrast -80 resources/gfx/sprites/intro.png resources/gfx/sprites/intro_2.png
convert -strip -brightness-contrast -90 resources/gfx/sprites/intro.png resources/gfx/sprites/intro_1.png

# n64 + n64brew jam logo
convert -strip -brightness-contrast -10 resources/gfx/sprites/n64brew_jam_logo.png resources/gfx/sprites/n64brew_jam_logo_9.png
convert -strip -brightness-contrast -20 resources/gfx/sprites/n64brew_jam_logo.png resources/gfx/sprites/n64brew_jam_logo_8.png
convert -strip -brightness-contrast -30 resources/gfx/sprites/n64brew_jam_logo.png resources/gfx/sprites/n64brew_jam_logo_7.png
convert -strip -brightness-contrast -40 resources/gfx/sprites/n64brew_jam_logo.png resources/gfx/sprites/n64brew_jam_logo_6.png
convert -strip -brightness-contrast -50 resources/gfx/sprites/n64brew_jam_logo.png resources/gfx/sprites/n64brew_jam_logo_5.png
convert -strip -brightness-contrast -60 resources/gfx/sprites/n64brew_jam_logo.png resources/gfx/sprites/n64brew_jam_logo_4.png
convert -strip -brightness-contrast -70 resources/gfx/sprites/n64brew_jam_logo.png resources/gfx/sprites/n64brew_jam_logo_3.png
convert -strip -brightness-contrast -80 resources/gfx/sprites/n64brew_jam_logo.png resources/gfx/sprites/n64brew_jam_logo_2.png
convert -strip -brightness-contrast -90 resources/gfx/sprites/n64brew_jam_logo.png resources/gfx/sprites/n64brew_jam_logo_1.png