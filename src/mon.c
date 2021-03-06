/*	SCCS Id: @(#)mon.c	3.4	2003/12/04	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

/* If you're using precompiled headers, you don't want this either */
#ifdef MICROPORT_BUG
#define MKROOM_H
#endif

#include "hack.h"
#include "mfndpos.h"
#include "edog.h"
#include "artifact.h"
#include "display.h"
#include "global.h" 
#include "quest.h"

#include <ctype.h>

void display_monster(XCHAR_P,XCHAR_P,struct monst *,int,XCHAR_P);
 
STATIC_DCL boolean restrap(struct monst *);
STATIC_DCL long mm_aggression(struct monst *,struct monst *);
#ifdef OVL2
STATIC_DCL int pick_animal(void);
STATIC_DCL int select_newcham_form(struct monst *);
STATIC_DCL void kill_eggs(struct obj *);
#endif

/* make wraith luring unnecessary --Amy */

#ifdef REINCARNATION
#define LEVEL_SPECIFIC_NOCORPSE(mdat) \
	 ( (Is_rogue_level(&u.uz) && rn2(2)) || \
	   ( (level.flags.graveyard || mdat == &mons[PM_WRAITH] || mdat == &mons[PM_NASTY_WRAITH] || mdat == &mons[PM_CREEPING___] ) && is_undead(mdat) && !is_reviver(mdat) && !(mdat == &mons[PM_TROLL_ZOMBIE]) && !(mdat == &mons[PM_EGO_TROLL_MUMMY]) && !(mdat == &mons[PM_TROLL_PERMAMIMIC_MUMMY]) && !(mdat == &mons[PM_TROLL_MUMMY]) && \
	    mdat != &mons[PM_VECNA] && rn2(3)))
#else
#define LEVEL_SPECIFIC_NOCORPSE(mdat) \
	   ((level.flags.graveyard || mdat == &mons[PM_WRAITH] || mdat == &mons[PM_NASTY_WRAITH] || mdat == &mons[PM_CREEPING___] ) && is_undead(mdat) && !is_reviver(mdat) && !(mdat == &mons[PM_TROLL_ZOMBIE]) && !(mdat == &mons[PM_EGO_TROLL_MUMMY]) && !(mdat == &mons[PM_TROLL_PERMAMIMIC_MUMMY]) && !(mdat == &mons[PM_TROLL_MUMMY]) && \
	    mdat != &mons[PM_VECNA] && rn2(3))
#endif

#define STARVATION_SPECIFIC_NOCORPSE(mdat) \
	 (!is_reviver(mdat) && !(mdat == &mons[PM_TROLL_ZOMBIE]) && !(mdat == &mons[PM_EGO_TROLL_MUMMY]) && !(mdat == &mons[PM_TROLL_PERMAMIMIC_MUMMY]) && !(mdat == &mons[PM_TROLL_MUMMY]))

#define Qstatf(x)	(quest_status.x)

#if 0
/* part of the original warning code which was replaced in 3.3.1 */
#ifdef OVL1
#define warnDelay 10
long lastwarntime;
int lastwarnlev;


STATIC_DCL void warn_effects(void);
#endif /* OVL1 */
#endif /* 0 */

#ifndef OVLB
STATIC_VAR short cham_to_pm[];
#else
STATIC_DCL struct obj *make_corpse(struct monst *);
STATIC_DCL void m_detach(struct monst *, struct permonst *);
STATIC_DCL void lifesaved_monster(struct monst *);
static void unpoly_monster(struct monst *);

/* convert the monster index of an undead to its living counterpart */
int
undead_to_corpse(mndx)
int mndx;
{
	switch (mndx) {
	case PM_KOBOLD_ZOMBIE:
	case PM_KOBOLD_MUMMY:	mndx = PM_KOBOLD;  break;
	case PM_OGRE_ZOMBIE:
	case PM_OGRE_MUMMY:	mndx = PM_OGRE;  break;
	case PM_DWARF_ZOMBIE:
	case PM_DWARF_MUMMY:	mndx = PM_DWARF;  break;
	case PM_GNOME_ZOMBIE:
	case PM_GNOME_MUMMY:	mndx = PM_GNOME;  break;
	case PM_ORC_ZOMBIE:
	case PM_ORC_MUMMY:	mndx = PM_ORC;  break;
	case PM_DROW_MUMMY:
	case PM_DROW_ZOMBIE:
	case PM_ELF_ZOMBIE:
	case PM_ELF_MUMMY:	mndx = PM_ELF;  break;
	case PM_VAMPIRE:
	case PM_VAMPIRE_LORD:
	case PM_VAMPIRE_MAGE:
	case PM_HUMAN_ZOMBIE:
	case PM_GRUNTHACK_ZOMBIE:
	case PM_GRUNT_A_LOT_ZOMBIE:
	case PM_BLACK_DEATH:
	case PM_DRAUGR_DEATHLORD:
	case PM_WALKING_CORPSE:
	case PM_HUNGRY_DEAD_POTATO:
	case PM_HUNGRY_DEAD:
	case PM_ROTTING_CARCASS:
	case PM_DROWNED_DEAD_ONE:
	case PM_FLESHEATER:
	case PM_PALE_SHAMBLER:
	case PM_EMBER_THRALL:
	case PM_FIERY_CARCASS:
	case PM_ZOMBIE_CAR:
	case PM_PLAGUE_BEARER:
	case PM_GREENER_ZOMBIE:
	case PM_ZOMBIE_BADDIE:
	case PM_ZOMBIE_WARRIOR:
	case PM_MAFIOSO:
	case PM_TALIBAN_MILITIA:
	case PM_M__TO_M___:
	case PM_ETA_TERRORIST:
	case PM_BLACK_ONES:
	case PM_CORNFLAKE_ZOMBIE:
	case PM_ZOMBIE:
	case PM_WEAUM_ZOMBIE:
	case PM_GNOLL_GHOUL:
	case PM_DRAUGR_ZOMBIE:
	case PM_STEEL_ZOMBIE:
	case PM_DRAUGR_MUMMY:
	case PM_GREATER_MUMMY:
	case PM_HALF_DRAGON_MUMMY:
	case PM_HALF_DRAGON_ZOMBIE:
	case PM_MUMMY:
	case PM_TOMB_MUMMY:
	case PM_CADAVER:
	case PM_POISONOUS_WIMP:
	case PM_PRESERVED_DEAD:
	case PM_RECEIVED_DEAD_POTATO:
	case PM_EMBALMED_CORPSE:
	case PM_TOXICATED_POTATO:
	case PM_DRIED_CORPSE:
	case PM_CORRUPTED_SHAMBLER:
	case PM_DEAD_PHARAO:
	case PM_DECAYED_CORPSE:
	case PM_WAIVED_POTATO:
	case PM_DEAD_POTATO:
	case PM_ADOM_MUMMY:
	case PM_HUMAN_MUMMY:	mndx = PM_HUMAN;  break;
	case PM_GIANT_ZOMBIE:
	case PM_GIANT_MUMMY:	mndx = PM_GIANT;  break;
	case PM_ETTIN_ZOMBIE:
	case PM_ETTIN_MUMMY:	mndx = PM_ETTIN;  break;
	case PM_TROLL_ZOMBIE:
	case PM_EGO_TROLL_MUMMY:
	case PM_TROLL_PERMAMIMIC_MUMMY:
	case PM_TROLL_MUMMY:    mndx = PM_TROLL;  break;
	case PM_MIMIC_MUMMY:    mndx = PM_GIANT_MIMIC;  break;
	case PM_TASMANIAN_ZOMBIE:    mndx = PM_TASMANIAN_DEVIL;  break;
	case PM_ZOMBIE_PROSTITUTE:    mndx = PM_NORMAL_PROSTITUTE;  break;
	default:  break;
	}
	return mndx;
}

/* Convert the monster index of some monsters (such as quest guardians)
 * to their generic species type.
 *
 * Return associated character class monster, rather than species
 * if mode is 1.
 */
int
genus(mndx, mode)
int mndx, mode;
{
	switch (mndx) {
/* Quest guardians */
	case PM_STUDENT:     mndx = mode ? PM_ARCHEOLOGIST  : PM_HUMAN; break;
	case PM_CHIEFTAIN:   mndx = mode ? PM_BARBARIAN : PM_HUMAN; break;
	case PM_NEANDERTHAL: mndx = mode ? PM_CAVEMAN   : PM_HUMAN; break;
	case PM_ATTENDANT:   mndx = mode ? PM_HEALER    : PM_HUMAN; break;
	case PM_UNFORTUNATE_VICTIM:   mndx = mode ? PM_BLEEDER    : PM_HUMAN; break;
	case PM_PAGE:        mndx = mode ? PM_KNIGHT    : PM_HUMAN; break;
	case PM_RHYMER:      mndx = mode ? PM_BARD   : PM_HUMAN; break;
	case PM_ABBOT:       mndx = mode ? PM_MONK      : PM_HUMAN; break;
	case PM_ACOLYTE:     mndx = mode ? PM_PRIEST    : PM_HUMAN; break;
	case PM_HUNTER:      mndx = mode ? PM_RANGER    : PM_HUMAN; break;
	case PM_THUG:        mndx = mode ? PM_ROGUE     : PM_HUMAN; break;
	case PM_ROSHI:       mndx = mode ? PM_SAMURAI   : PM_HUMAN; break;
	case PM_GUIDE:       mndx = mode ? PM_TOURIST   : PM_HUMAN; break;
	case PM_APPRENTICE:  mndx = mode ? PM_WIZARD    : PM_HUMAN; break;
	case PM_SUPER_WARRIOR:     mndx = mode ? PM_VALKYRIE  : PM_HUMAN; break;
	default:
		if (mndx >= LOW_PM && mndx < NUMMONS) {
			struct permonst *ptr = &mons[mndx];
			if (is_human(ptr))      mndx = PM_HUMAN;
			else if (is_elf(ptr))   mndx = PM_ELF;
			else if (is_dwarf(ptr)) mndx = PM_DWARF;
			else if (is_gnome(ptr)) mndx = PM_GNOME;
			else if (is_orc(ptr))   mndx = PM_ORC;
		}
		break;
	}
	return mndx;
}

/* convert monster index to chameleon index */
int
pm_to_cham(mndx)
int mndx;
{
	int mcham;

	switch (mndx) {
	case PM_CHAMELEON:	mcham = CHAM_CHAMELEON; break;
	case PM_DOPPELGANGER:	mcham = CHAM_DOPPELGANGER; break;
	case PM_DOPPLEZON:	mcham = CHAM_DOPPLEZON; break;
	case PM_SANDESTIN:	mcham = CHAM_SANDESTIN; break;
	case PM_MISSINGNO:	mcham = CHAM_MISSINGNO; break;
	case PM_TRANSFORMER:	mcham = CHAM_TRANSFORMER; break;
	case PM_WARPER:	mcham = CHAM_WARPER; break;
	case PM_CHAOS_SHAPECHANGER:	mcham = CHAM_CHAOS_SHAPECHANGER; break;
	case PM_GIANT_CHAMELEON:	mcham = CHAM_GIANT_CHAMELEON; break;
	default: mcham = CHAM_ORDINARY; break;
	}
	return mcham;
}

/* convert chameleon index to monster index */
STATIC_VAR short cham_to_pm[] = {
		NON_PM,		/* placeholder for CHAM_ORDINARY */
		PM_CHAMELEON,
		PM_DOPPELGANGER,
		PM_DOPPLEZON,
		PM_SANDESTIN,
		PM_MISSINGNO,
		PM_TRANSFORMER,
		PM_WARPER,
		PM_CHAOS_SHAPECHANGER,
		PM_GIANT_CHAMELEON,
};

/* for deciding whether corpse or statue will carry along full monster data */
#define KEEPTRAITS(mon)	((mon)->isshk || (mon)->isgyp || (mon)->mtame ||\
			 ((mon)->data->geno & G_UNIQ) ||		\
			 (mon)->egotype_troll ||	\
			 is_reviver((mon)->data) ||			\
			 /* normally leader the will be unique, */	\
			 /* but he might have been polymorphed  */	\
			 (mon)->m_id == quest_status.leader_m_id ||	\
			 /* special cancellation handling for these */	\
			 (dmgtype((mon)->data, AD_SEDU) ||		\
			  dmgtype((mon)->data, AD_SSEX)))

/* Creates a monster corpse, a "special" corpse, or nothing if it doesn't
 * leave corpses.  Monsters which leave "special" corpses should have
 * G_NOCORPSE set in order to prevent wishing for one, finding tins of one,
 * etc....
 */
STATIC_OVL struct obj *
make_corpse(mtmp)
register struct monst *mtmp;
{
	register struct permonst *mdat = mtmp->data;
	int num;
	struct obj *obj = (struct obj *)0;
	int x = mtmp->mx, y = mtmp->my;
	int mndx = monsndx(mdat);

	switch(mndx) {
	    case PM_GRAY_DRAGON:
	    case PM_MERCURIAL_DRAGON:
	    case PM_SILVER_DRAGON:
	    case PM_SHIMMERING_DRAGON:
	    case PM_DEEP_DRAGON:
	    case PM_RED_DRAGON:
	    case PM_ORANGE_DRAGON:
	    case PM_WHITE_DRAGON:
	    case PM_BLACK_DRAGON:
	    case PM_BLUE_DRAGON:
	    case PM_COPPER_DRAGON:
	    case PM_PLATINUM_DRAGON:
	    case PM_BRASS_DRAGON:
	    case PM_AMETHYST_DRAGON:
	    case PM_PURPLE_DRAGON:
	    case PM_DIAMOND_DRAGON:
	    case PM_EMERALD_DRAGON:
	    case PM_SAPPHIRE_DRAGON:
	    case PM_RUBY_DRAGON:
	    case PM_GREEN_DRAGON:
	    case PM_GOLDEN_DRAGON:
	    case PM_STONE_DRAGON:
	    case PM_CYAN_DRAGON:
	    case PM_PSYCHIC_DRAGON:
	    case PM_RAINBOW_DRAGON:
	    case PM_BLOOD_DRAGON:
	    case PM_PLAIN_DRAGON:
	    case PM_SKY_DRAGON:
	    case PM_WATER_DRAGON:
	    case PM_EVIL_DRAGON:
	    case PM_MAGIC_DRAGON:
	    case PM_YELLOW_DRAGON:
		/* Make dragon scales.  This assumes that the order of the */
		/* dragons is the same as the order of the scales.	   */
		if (!rn2(mtmp->mrevived ? 20 : 3)) {
		    num = GRAY_DRAGON_SCALES + monsndx(mdat) - PM_GRAY_DRAGON;
		    if (!rn2(8)) num = GRAY_DRAGON_SCALE_SHIELD + monsndx(mdat) - PM_GRAY_DRAGON;
		    obj = mksobj_at(num, x, y, TRUE, FALSE); /* allow random enchantment and BUC --Amy */
		    /*obj->spe = 0;
		    obj->cursed = obj->blessed = FALSE;*/
		}
		goto default_1;

	    case PM_YOUNG_UNICORN:
		if (rn2(2)) goto default_1;
		/* fall through */
	    case PM_WHITE_UNICORN:
	    case PM_WINGED_WHITE_UNICORN:
	    case PM_WINGED_GRAY_UNICORN:
	    case PM_WINGED_BLACK_UNICORN:
	    case PM_BEIGE_UNICORN:
	    case PM_CONCRETE_UNICORN:
	    case PM_ANTHRAZITE_UNICORN:
	    case PM_SMOKY_QUARTZ_UNICORN:
	    case PM_JET_UNICORN:
	    case PM_PEARL_UNICORN:
	    case PM_PURE_WHITE_UNICORN:
	    case PM_WHITE_UNICORN_FOAL:
	    case PM_GRAY_UNICORN_FOAL:
	    case PM_BLACK_UNICORN_FOAL:
	    case PM_GRAY_UNICORN:
	    case PM_PLAYER_UNICORN:
	    case PM_HANDSOME_GRAY_UNICORN:
	    case PM_BLACK_UNICORN:
	    case PM_PORTER_BLACK_UNICORN:
	    case PM_PURPLE_UNICORN:
	    case PM_PINK_UNICORN:
	    case PM_GOLDEN_UNICORN:
	    case PM_UNICORN_SKELETON:
	    case PM_NIGHTMARE_UNICORN:
	    case PM_UNICORN_OF_AMBER:
	    case PM_RAINBOW_UNICORN:
	    case PM_DESATURATED_UNICORN:
	    case PM_DULL_UNICORN:
	    case PM_POWERLESS_UNICORN:
	    case PM_BIG_PURPLE_UNICORN:
	    case PM_BIG_WHITE_UNICORN:
	    case PM_BIG_GRAY_UNICORN:
	    case PM_BIG_BLACK_UNICORN:
	    case PM_SQUIRRELHORN:
	    case PM_PETTY_BIG_WHITE_UNICORN:
	    case PM_PETTY_BIG_GRAY_UNICORN:
	    case PM_PETTY_BIG_BLACK_UNICORN:
		if (mtmp->mrevived && rn2(20)) {
			if (canseemon(mtmp))
			   pline("%s recently regrown horn crumbles to dust.",
				s_suffix(Monnam(mtmp)));
		} else
			(void) mksobj_at(UNICORN_HORN, x, y, TRUE, FALSE);
		goto default_1;
	    case PM_LONG_WORM:
		(void) mksobj_at(WORM_TOOTH, x, y, TRUE, FALSE);
		goto default_1;
	    case PM_KILLER_TRIPE_RATION:
	    case PM_PERSONALIZED_KILLER_TRIPE_RATION:
		(void) mksobj_at(TRIPE_RATION, x, y, TRUE, FALSE);
		newsym(x, y);
		return (struct obj *)0;
	    case PM_KILLER_APPLE:
	    case PM_IWBTG_APPLE:
		(void) mksobj_at(APPLE, x, y, TRUE, FALSE);
		newsym(x, y);
		return (struct obj *)0;
	    case PM_EVIL_ORANGE:
		(void) mksobj_at(ORANGE, x, y, TRUE, FALSE);
		newsym(x, y);
		return (struct obj *)0;
	    case PM_FLOATING_LEMON:
		(void) mksobj_at(LEMON, x, y, TRUE, FALSE);
		newsym(x, y);
		return (struct obj *)0;
	    case PM_FLOATING_MELON:
		(void) mksobj_at(MELON, x, y, TRUE, FALSE);
		newsym(x, y);
		return (struct obj *)0;
	    case PM_RAVENOUS_CREAM_PIE:
	    case PM_KOP_BRANDED_RAVENOUS_CREAM_PIE:
		(void) mksobj_at(CREAM_PIE, x, y, TRUE, FALSE);
		newsym(x, y);
		return (struct obj *)0;
	    case PM_KILLER_FOOD_RATION:
	    case PM_ROTTEN_FOOD_RATION:
		(void) mksobj_at(FOOD_RATION, x, y, TRUE, FALSE);
		newsym(x, y);
		return (struct obj *)0;
	    case PM_BAD_EGG:
	    case PM_PORTER_BAD_EGG:
		(void) mksobj_at(EGG, x, y, TRUE, FALSE);
		newsym(x, y);
		return (struct obj *)0;
	    case PM_VAMPIRE:
	    case PM_VAMPIRE_LORD:
	    case PM_VAMPIRE_MAGE:
		/* include mtmp in the mkcorpstat() call */
		num = undead_to_corpse(mndx);
		obj = mkcorpstat(CORPSE, mtmp, &mons[num], x, y, TRUE);
		obj->age -= 100;		/* this is an *OLD* corpse */
		break;
	    case PM_KOBOLD_MUMMY:
	    case PM_DWARF_MUMMY:
	    case PM_GNOME_MUMMY:
	    case PM_ORC_MUMMY:
	    case PM_ELF_MUMMY:
	    case PM_HUMAN_MUMMY:
	    case PM_MUMMY:
		case PM_TOMB_MUMMY:
	    case PM_ADOM_MUMMY:
	    case PM_MIMIC_MUMMY:
	    case PM_GIANT_MUMMY:
	    case PM_ETTIN_MUMMY:
	    case PM_TROLL_MUMMY:            
	    case PM_EGO_TROLL_MUMMY:            
	    case PM_TROLL_PERMAMIMIC_MUMMY:
	    case PM_TROLL_ZOMBIE:            
	    case PM_KOBOLD_ZOMBIE:
	    case PM_OGRE_ZOMBIE:
	    case PM_JUJU_ZOMBI:
	    case PM_OGRE_MUMMY:
	    case PM_DWARF_ZOMBIE:
	    case PM_GNOME_ZOMBIE:
	    case PM_ORC_ZOMBIE:
	    case PM_ELF_ZOMBIE:
	    case PM_HUMAN_ZOMBIE:
	case PM_GRUNTHACK_ZOMBIE:
	case PM_GRUNT_A_LOT_ZOMBIE:
	case PM_CADAVER:
	case PM_POISONOUS_WIMP:
	case PM_PRESERVED_DEAD:
	case PM_RECEIVED_DEAD_POTATO:
	case PM_EMBALMED_CORPSE:
	case PM_TOXICATED_POTATO:
	case PM_CORRUPTED_SHAMBLER:
	case PM_DRIED_CORPSE:
	case PM_DEAD_PHARAO:
	case PM_DECAYED_CORPSE:
	case PM_WAIVED_POTATO:
	case PM_DEAD_POTATO:
	    case PM_BLACK_DEATH:
	    case PM_DRAUGR_DEATHLORD:
	    case PM_TASMANIAN_ZOMBIE:
	    case PM_WALKING_CORPSE:
	case PM_HUNGRY_DEAD_POTATO:
	case PM_HUNGRY_DEAD:
	case PM_ROTTING_CARCASS:
	case PM_DROWNED_DEAD_ONE:
	case PM_FLESHEATER:
	case PM_PALE_SHAMBLER:
	case PM_EMBER_THRALL:
	case PM_FIERY_CARCASS:
	case PM_ZOMBIE_CAR:
	case PM_PLAGUE_BEARER:
	case PM_GREENER_ZOMBIE:
	    case PM_ZOMBIE_BADDIE:
	case PM_ZOMBIE_WARRIOR:
	    case PM_MAFIOSO:
	case PM_TALIBAN_MILITIA:
	case PM_M__TO_M___:
	    case PM_ETA_TERRORIST:
	    case PM_BLACK_ONES:
	    case PM_CORNFLAKE_ZOMBIE:
	    case PM_ZOMBIE:
	case PM_WEAUM_ZOMBIE:
	    case PM_GIANT_ZOMBIE:
	    case PM_ETTIN_ZOMBIE:
		case PM_GNOLL_GHOUL:
	    case PM_DRAUGR_ZOMBIE:
	    case PM_STEEL_ZOMBIE:
	    case PM_DRAUGR_MUMMY:
	    case PM_GREATER_MUMMY:
	    case PM_DROW_ZOMBIE:
	    case PM_ZOMBIE_PROSTITUTE:
	    case PM_DROW_MUMMY:
	    case PM_HALF_DRAGON_MUMMY:
	    case PM_HALF_DRAGON_ZOMBIE:
		num = undead_to_corpse(mndx);
		obj = mkcorpstat(CORPSE, mtmp, &mons[num], x, y, TRUE);
		obj->age -= 100;		/* this is an *OLD* corpse */
		break;
	    case PM_WIGHT:
	    case PM_FOREST_WIGHT:
	    case PM_GRAVE_WIGHT:
	    case PM_EMPEROR_WIGHT:
	    case PM_MEANIE:
	    case PM_CAR_THIEF:
	    case PM_WATER_WIGHT:
	    case PM_SUBWAY_WIGHT:
	    case PM_CRYPT_WIGHT:
	    case PM_BARROW_WIGHT:
	    case PM_HIDDEN_BARROW_WIGHT:
	    case PM_TUNNEL_WIGHT:
	    case PM_TUNNEL_FORCE_WIGHT:
	    case PM_PORTER_WIGHT:
	    case PM_TRAILER:
	    case PM_UNDEAD_BAT:
	    case PM_UNDEAD_SWARM_BAT:
	    case PM_PETTY_TUNNEL_WIGHT:
	    case PM_CASTLE_WIGHT:
	    case PM_GRAVE_WARRIOR:
	    case PM_ULTRA_WIGHT:
	    case PM_GHOUL:
	    case PM_GHAST:
	    case PM_GASTLY:
	    case PM_HAUNTER:
	    case PM_GENGAR:
	    case PM_CREEPING___:
	    case PM_FRANKENSTEIN_S_MONSTER:
	    case PM_UNDEAD_MIMIC:
	    case PM_UNDEAD_PERMAMIMIC:
	    case PM_UNDEAD_SWARM_MIMIC:
	    case PM_UNDEAD_SWARM_PERMAMIMIC:
	    case PM_UNDEAD_KANGAROO:
	    case PM_RIBBON_FISH:
	    case PM_SKELETON_FISH:
	    case PM_BONE_FISH:
	    case PM_CORPSE_FISH:
	    case PM_HORROR_FISH:
	    case PM_UNDEAD_OLOG_HAI_AMBUSHER:
	    case PM_UNDEAD_ANT:
	    case PM_UNDEAD_COCKATRICE:
	    case PM_MUTATED_UNDEAD_COCKATRICE:
	    case PM_SKELETAL_HOUND:
	    case PM_CORPSE_HOUND:
	    case PM_REAL_CORPSE_HOUND:
	    case PM_ZOMBOCAT:
	    case PM_HAUNTED_TIGER:
	    case PM_UNDEAD_MIND_FLAYER:
	    case PM_ELDRITCH_MIND_FLAYER:
	    case PM_UNDEAD_NYMPH:
	    case PM_SKELETTOCROTTA:
	    case PM_UNDEAD_RAT:
	    case PM_UNDEAD_SCORPION:
	    case PM_UNDEAD_GNASHER_SCORPION:
	    case PM_UNDEAD_HORSE:
	    case PM_UNDEAD_UNICORN: /* too decayed to leave a working horn, so we just don't leave one at all :D --Amy */
	    case PM_DISGUISED_UNDEAD_UNICORN:
	    case PM_UNDEAD_CENTAUR:
	    case PM_FLYING_UNDEAD_CENTAUR:
	    case PM_UNDEAD_KOP:
	    case PM_UNDEAD_SERGEANT:
	    case PM_UNDEAD_LIEUTENANT:
	    case PM_UNDEAD_KOMMISSIONER:
	    case PM_UNDEAD_KAPTAIN:
	    case PM_UNDEAD_KCHIEF:
	    case PM_UNDEAD_KATCHER:
	    case PM_UNDEAD_KRIMINOLOGIST:
	    case PM_UNDEAD_SNAKE:
	    case PM_UNDEAD_EGO_SNAKE:
	    case PM_UNDEAD_NAGA:
	    case PM_UNDEAD_NAGA_HATCHLING:
	    case PM_UNDEAD_YETI:
	    case PM_UNDEAD_ARCHEOLOGIST:
	    case PM_UNDEAD_BARBARIAN:
	    case PM_UNDEAD_BINDER:
	    case PM_UNDEAD_BLEEDER:
	    case PM_UNDEAD_BARD:
	    case PM_UNDEAD_CAVEMAN:
	    case PM_UNDEAD_CAVEWOMAN:
	    case PM_UNDEAD_CHEVALIER:
	    case PM_UNDEAD_COURIER:
	    case PM_UNDEAD_SPACEWARS_FIGHTER:
	    case PM_UNDEAD_CAMPERSTRIKER:
	    case PM_UNDEAD_ZYBORG:
	    case PM_UNDEAD_DEATH_EATER:
	    case PM_UNDEAD_GANGSTER:
	    case PM_UNDEAD_POKEMON:
	    case PM_UNDEAD_ELECTRIC_MAGE:
	    case PM_UNDEAD_POISON_MAGE:
	    case PM_UNDEAD_OCCULT_MASTER:
	    case PM_UNDEAD_CHAOS_SORCEROR:
	    case PM_UNDEAD_ELEMENTALIST:
	    case PM_UNDEAD_WILD_TALENT:
	    case PM_UNDEAD_ACID_MAGE:
	    case PM_UNDEAD_FLAME_MAGE:
	    case PM_UNDEAD_CONVICT:
	    case PM_UNDEAD_LUNATIC:
	    case PM_UNDEAD_GEEK:
	    case PM_UNDEAD_GRADUATE:
	    case PM_UNDEAD_SCIENTIST:
	    case PM_UNDEAD_ROCKER:
	    case PM_UNDEAD_HEALER:
	    case PM_UNDEAD_ICE_MAGE:
	    case PM_UNDEAD_KNIGHT:
	    case PM_UNDEAD_WARRIOR:
	    case PM_UNDEAD_MONK:
	    case PM_UNDEAD_PROSTITUTE:
	    case PM_UNDEAD_KURWA:
	    case PM_UNDEAD_PSION:
	    case PM_UNDEAD_NECROMANCER:
	    case PM_UNDEAD_NOBLEMAN:
	    case PM_UNDEAD_NOBLEWOMAN:
	    case PM_UNDEAD_PIRATE:
	    case PM_UNDEAD_KORSAIR:
	    case PM_UNDEAD_CRUEL_ABUSER:
	    case PM_UNDEAD_SAIYAN:
	    case PM_UNDEAD_POLITICIAN:
	    case PM_UNDEAD_LIBRARIAN:
	    case PM_UNDEAD_COOK:
	    case PM_UNDEAD_AUGURER:
	    case PM_UNDEAD_SAGE:
	    case PM_UNDEAD_GUNNER:
	    case PM_UNDEAD_OTAKU:
	    case PM_UNDEAD_ARTIST:
	    case PM_UNDEAD_GAMER:
	    case PM_UNDEAD_DOLL_MISTRESS:
	    case PM_UNDEAD_FEAT_MASTER:
	    case PM_UNDEAD_FOXHOUND_AGENT:
	    case PM_UNDEAD_INTEL_SCRIBE:
	    case PM_UNDEAD_PALADIN:
	    case PM_UNDEAD_WANDKEEPER:
	    case PM_UNDEAD_ASSASSIN:
	    case PM_UNDEAD_BULLY:
	    case PM_UNDEAD_SUPERMARKET_CASHIER:
	    case PM_UNDEAD_SLAVE_MASTER:
	    case PM_UNDEAD_MYSTIC:
	    case PM_UNDEAD_BLOODSEEKER:
	    case PM_UNDEAD_LADIESMAN:
	    case PM_UNDEAD_JESTER:
	    case PM_UNDEAD_PICKPOCKET:
	    case PM_UNDEAD_MAHOU_SHOUJO:
	    case PM_UNDEAD_DIVER:
	    case PM_UNDEAD_GLADIATOR:
	    case PM_UNDEAD_GOFF:
	    case PM_UNDEAD_AMAZON:
	    case PM_UNDEAD_MURDERER:
	    case PM_UNDEAD_ALTMER:
	    case PM_UNDEAD_BOSMER:
	    case PM_UNDEAD_DUNMER:
	    case PM_UNDEAD_THALMOR:
	    case PM_UNDEAD_ORDINATOR:
	    case PM_UNDEAD_PRIEST:
	    case PM_UNDEAD_PRIESTESS:
	    case PM_UNDEAD_RANGER:
	    case PM_UNDEAD_ELPH:
	    case PM_UNDEAD_ROGUE:
	    case PM_UNDEAD_SAMURAI:
	    case PM_UNDEAD_TOURIST:
	    case PM_UNDEAD_DRUNK:
	    case PM_UNDEAD_FIREFIGHTER:
	    case PM_UNDEAD_LOCKSMITH:
	    case PM_UNDEAD_ERDRICK:
	    case PM_UNDEAD_FIGHTER:
	    case PM_UNDEAD_FENCER:
	    case PM_UNDEAD_MEDIUM:
	    case PM_UNDEAD_SEXYMATE:
	    case PM_UNDEAD_STAND_USER:
	    case PM_UNDEAD_JUSTICE_KEEPER:
	    case PM_UNDEAD_DOOM_MARINE:
	    case PM_UNDEAD_DQ_SLIME:
	    case PM_UNDEAD_MUSICIAN:
	    case PM_UNDEAD_FAILED_EXISTENCE:
	    case PM_UNDEAD_NINJA:
	    case PM_UNDEAD_OFFICER:
	    case PM_UNDEAD_UNDERTAKER:
	    case PM_UNDEAD_ZOOKEEPER:
	    case PM_UNDEAD_TRANSVESTITE:
	    case PM_UNDEAD_TRANSSYLVANIAN:
	    case PM_UNDEAD_UNBELIEVER:
	    case PM_UNDEAD_TOPMODEL:
	    case PM_UNDEAD_ACTIVISTOR:
	    case PM_NON_UNDEAD_SLAYER:
	    case PM_UNDEAD_VALKYRIE:
	    case PM_UNDEAD_YEOMAN:
	    case PM_UNDEAD_PADAWAN:
	    case PM_UNDEAD_JEDI:
	    case PM_UNDEAD_WIZARD:
	    case PM_UNDEAD_MOLD:
	    case PM_FUNGOID_MOLD:
	    case PM_UNDEAD_FUNGUS:
	    case PM_MOLDY_FUNGUS:
	    case PM_UNDEAD_PATCH:
	    case PM_MOLDY_PATCH:
	    case PM_UNDEAD_FORCE_FUNGUS:
	    case PM_MOLDY_FORCE_FUNGUS:
	    case PM_UNDEAD_FORCE_PATCH:
	    case PM_MOLDY_FORCE_PATCH:
	    case PM_UNDEAD_WARP_FUNGUS:
	    case PM_MOLDY_WARP_FUNGUS:
	    case PM_UNDEAD_WARP_PATCH:
	    case PM_MOLDY_WARP_PATCH:
	    case PM_UNDEAD_STALK:
	    case PM_MOLDY_STALK:
	    case PM_UNDEAD_GROWTH:
	    case PM_FUNGOID_GROWTH:
	    case PM_UNDEAD_SPORE:
	    case PM_MOLDY_SPORE:
	    case PM_UNDEAD_MUSHROOM:
	    case PM_MOLDY_MUSHROOM:
	    case PM_UNDEAD_COLONY:
	    case PM_MOLDY_COLONY:
	    case PM_MUTATED_UNDEAD_POTATO:
		obj = mkcorpstat(CORPSE, (struct monst *)0, &mons[mndx], x, y, TRUE);
		obj->age -= 100;                /* this is an *OLD* corpse */
		break;
	    case PM_MEDUSA: {
			struct monst *mtmp2;


			/* KMH -- the legend of Medusa and Pegasus */
			/* Only when Medusa leaves a corpse */
			mtmp2 = makemon(&mons[PM_PEGASUS], x, y, 0);
			if (mtmp2) {
				You("%s something spring forth from the corpse of %s.",
						Blind ? "sense" : "see", mon_nam(mtmp));
				mtmp2->mpeaceful = 1;
				mtmp2->mtame = 0;
			}
			goto default_1;
		}
	    case PM_NIGHTMARE:
			pline("All that remains is her horn...");
			obj = oname(mksobj(UNICORN_HORN, TRUE, FALSE),
					artiname(ART_NIGHTHORN));
			goto initspecial;
	    case PM_BEHOLDER:
			pline("All that remains is a single eye...");
			obj = oname(mksobj(EYEBALL, TRUE, FALSE),
					artiname(ART_EYE_OF_THE_BEHOLDER));
			goto initspecial;
	    case PM_VECNA:
			pline("All that remains is a hand...");
			obj = oname(mksobj(SEVERED_HAND, TRUE, FALSE),
					artiname(ART_HAND_OF_VECNA));
		initspecial:
			obj->quan = 1;
			curse(obj);
			place_object(obj, x, y);
			stackobj(obj);
			newsym(x, y);
			return obj;
		break;
	    case PM_IRON_GOLEM:
		num = d(2,6);
		while (num--)
			obj = mksobj_at(IRON_CHAIN, x, y, TRUE, FALSE);
		mtmp->mnamelth = 0;
		break;
	    case PM_GLASS_GOLEM:
		num = d(2,4);   /* very low chance of creating all glass gems */
		while (num--)
			obj = mksobj_at((LAST_GEM + rnd(9)), x, y, TRUE, FALSE);
		mtmp->mnamelth = 0;
		break;
	    case PM_RUBY_GOLEM:
		/* [DS] Mik's original Lethe fobbed off the player with coloured
		 * glass even for the higher golems. We'll play fair here - if
		 * you can kill one of these guys, you deserve the gems. */
		num = d(2,4);
		while (num--)
			obj = mksobj_at(RUBY, x, y, TRUE, FALSE);
		mtmp->mnamelth = 0;
		break;
	    case PM_DIAMOND_GOLEM:
		num = d(2,4);   
		while (num--)
			obj = mksobj_at(DIAMOND, x, y, TRUE, FALSE);
		mtmp->mnamelth = 0;
		break;
	    case PM_SAPPHIRE_GOLEM:
		num = d(2,4);
		while (num--)
			obj = mksobj_at(SAPPHIRE, x, y, TRUE, FALSE);
		mtmp->mnamelth = 0;
		break;
	    case PM_STEEL_GOLEM:
		num = d(2,6);
		/* [DS] Add steel chains (or handcuffs!) for steel golems? */
		while (num--)
			obj = mksobj_at(IRON_CHAIN, x, y, TRUE, FALSE);
		mtmp->mnamelth = 0;
		break;
	    case PM_CRYSTAL_GOLEM:
		/* [DS] Generate gemstones of various hues */
		num = d(2,4);
		{
		    int gemspan = LAST_GEM - bases[GEM_CLASS] + 1;
		    while (num--)
			obj = mksobj_at(bases[GEM_CLASS] + rn2(gemspan), x, y,
		    			TRUE, FALSE);
		    mtmp->mnamelth = 0;
		}
		break;
	    case PM_CLAY_GOLEM:
		obj = mksobj_at(ROCK, x, y, FALSE, FALSE);

		if (obj) {
			if(!rn2(8)) {
				obj->spe = rne(2);
				if (rn2(2)) obj->blessed = rn2(2);
				 else	blessorcurse(obj, 3);
			} else if(!rn2(10)) {
				if (rn2(10)) curse(obj);
				 else	blessorcurse(obj, 3);
				obj->spe = -rne(2);
			} else	blessorcurse(obj, 10);
	
			obj->quan = (long)(rn2(20) + 50);
			obj->owt = weight(obj);
		}
		mtmp->mnamelth = 0;
		
		break;
	    case PM_STONE_GOLEM:
		obj = mkcorpstat(STATUE, (struct monst *)0,
			mdat, x, y, FALSE);
		break;
	    case PM_WOOD_GOLEM:
		num = d(2,4);
		while(num--)
			obj = mksobj_at(QUARTERSTAFF, x, y, TRUE, FALSE);
		mtmp->mnamelth = 0;
		break;
	    case PM_LEATHER_GOLEM:
		num = d(2,4);
		while(num--)
			obj = mksobj_at(LEATHER_ARMOR, x, y, TRUE, FALSE);
		mtmp->mnamelth = 0;
		break;
	    case PM_BUG:
	    	if (!rn2(6)) {
			mksobj_at(PACK_OF_FLOPPIES, x, y, TRUE, FALSE);
		}
		break;
	    case PM_HEISENBUG:
	    	if (!rn2(3)) {
			mksobj_at(PACK_OF_FLOPPIES, x, y, TRUE, FALSE);
		}
		break;
	    case PM_PDP___:	/* PDP-11 */
	    case PM_PDP__:	/* PDP-9 */
	    case PM_VAX:
	    case PM_CRAY:
	    	num = rn2(10) + 5;
		while (num--) {
			mksobj_at(!rn2(3) ? DIODE : (!rn2(2) ? TRANSISTOR : IC), x, y, TRUE, FALSE);
		}
		break;
	    case PM_ALGOLIAN_SUNTIGER:
		if (!rn2(75)) {
 			mksobj_at(TOOTH_OF_AN_ALGOLIAN_SUNTIGER,x, y, TRUE, FALSE);
		}
		goto default_1;
		case PM_WAX_GOLEM:
		num = d(2,4);
		while (num--)
			obj = mksobj_at(WAX_CANDLE, x, y, TRUE, FALSE);
		mtmp->mnamelth = 0;
		break;
	    case PM_PLASTIC_GOLEM:
		/* KMH -- Credit cards are #ifdef TOURIST */
		num = d(2,2);
		while (num--)
			obj = mksobj_at(
				CREDIT_CARD,
					x, y, TRUE, FALSE);
		mtmp->mnamelth = 0;
		break;
	    case PM_GOLD_GOLEM:
	    case PM_GOLD_SLIME:
		/* Good luck gives more coins */
		obj = mkgold((long)(200 - rnl(101)), x, y);
		mtmp->mnamelth = 0;
		break;
	    case PM_INVULNERABLE_GOLEM:
		/* Good luck gives more coins */
		obj = mkgold((long)(rnz(1000) + 2000 - rnl(1001)), x, y);
		mtmp->mnamelth = 0;
		break;

	    case PM_PAPER_GOLEM:
		num = rnd(4);
		while (num--)
			obj = mksobj_at(SCR_BLANK_PAPER, x, y, TRUE, FALSE);
		mtmp->mnamelth = 0;
		break;
	    default_1:
	    default: /* worm that walks has a chance to get a corpse even from G_NOCORPSE monsters --Amy */

		if ( (( (mvitals[mndx].mvflags & G_NOCORPSE) && !(mtmp->egotype_troll) ) || mtmp->egotype_multiplicator ) && ( ((!Race_if(PM_WORM_THAT_WALKS) || !polyok(mdat)) && !Race_if(PM_WARPER)) || (rn2(5) && !(mtmp->egotype_troll) ) || mndx == PM_ITEM_MASTER || mndx == PM_HOLE_MASTER || mndx == PM_TRAP_MASTER || mndx == PM_BOULDER_MASTER || mndx == PM_GOOD_ITEM_MASTER || mndx == PM_BAD_ITEM_MASTER ) )
		    return (struct obj *)0;
		else	/* preserve the unique traits of some creatures */
		    obj = mkcorpstat(CORPSE, KEEPTRAITS(mtmp) ? mtmp : 0,
				     mdat, x, y, TRUE);
		break;
	}

	if (!obj) return (struct obj *)0; /* really fixing the damn bug with the "bug" and similar monster types! --Amy */

	/* All special cases should precede the G_NOCORPSE check */

	/* if polymorph or undead turning has killed this monster,
	   prevent the same attack beam from hitting its corpse */
	if (flags.bypasses) bypass_obj(obj);

	if (mtmp->mnamelth)
	    obj = oname(obj, NAME(mtmp));

	/* Avoid "It was hidden under a green mold corpse!" 
	 *  during Blind combat. An unseen monster referred to as "it"
	 *  could be killed and leave a corpse.  If a hider then hid
	 *  underneath it, you could be told the corpse type of a
	 *  monster that you never knew was there without this.
	 *  The code in hitmu() substitutes the word "something"
	 *  if the corpses obj->dknown is 0.
	 */
	if (Blind && !sensemon(mtmp)) obj->dknown = 0;

	/* Invisible monster ==> invisible corpse */
	obj->oinvis = mtmp->perminvis;
	obj->oinvisreal = mtmp->minvisreal;

	stackobj(obj);
	newsym(x, y);
	return obj;
}

#endif /* OVLB */
#ifdef OVL1

#if 0
/* part of the original warning code which was replaced in 3.3.1 */
STATIC_OVL void
warn_effects()
{
	register struct monst *mtmp;    
	int num_mon;
	int warned_of;

	num_mon = 0;
	for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
		if (!canseemon(mtmp) && !(mtmp->mpeaceful) && !(mtmp->mtame)) {
			if (Warning && (!spec_ability(uwep,SPFX_WARN) || !spec_dbon(uwep,mtmp,1))) num_mon++;
			else if (Undead_warning && (is_undead(mtmp->data) || mtmp->egotype_undead) ) num_mon++;
			else if (spec_ability(uwep,SPFX_WARN) && spec_dbon(uwep,mtmp,1)) num_mon++;
	    }
	}
	if (num_mon == 0) return;
	/* num_mon is the # of monsters which could be warned against...
	pick one at random... */
	warned_of = rn2(num_mon)+1;
	mtmp = fmon;
	num_mon = warned_of;
	do {
		if (!canseemon(mtmp) && !(mtmp->mpeaceful) && !(mtmp->mtame)) {
			if (Warning && (!spec_ability(uwep,SPFX_WARN) || !spec_dbon(uwep,mtmp,1))) num_mon--;
			else if (Undead_warning && (is_undead(mtmp->data) || mtmp->egotype_undead) ) num_mon--;
			else if (spec_ability(uwep,SPFX_WARN) && spec_dbon(uwep,mtmp,1)) num_mon--;
		}
		if (num_mon > 0) mtmp = mtmp->nmon;
	} while (num_mon > 0);
	display_monster(mtmp->mx,mtmp->my,mtmp,1,0);
	lastwarnlev = warnlevel;
	lastwarntime = moves;
/*	warnlevel = 0;*/
	return;
}
#endif /* 0 */

/* check mtmp and water/lava for compatibility, 0 (survived), 1 (died) */
int
minliquid(mtmp)
register struct monst *mtmp;
{
    boolean inpool, inlava, infountain;

    inpool = (is_waterypool(mtmp->mx,mtmp->my) || is_watertunnel(mtmp->mx,mtmp->my)) &&
	     !is_flyer(mtmp->data) && !(is_urinelake(mtmp->mx,mtmp->my)) && !(is_moorland(mtmp->mx,mtmp->my)) && !(is_crystalwater(mtmp->mx,mtmp->my)) && (!mtmp->egotype_flying) && !is_floater(mtmp->data);
    inlava = is_lava(mtmp->mx,mtmp->my) &&
	     !is_flyer(mtmp->data) && (!mtmp->egotype_flying) && !is_floater(mtmp->data);
    infountain = IS_FOUNTAIN(levl[mtmp->mx][mtmp->my].typ);

	/* Flying and levitation keeps our steed out of the liquid */
	/* (but not water-walking or swimming) */
	if (mtmp == u.usteed && (Flying || Levitation))
		return (0);

	/* poisonous non-zombies will poison a well --Amy */
    if (IS_WELL(levl[mtmp->mx][mtmp->my].typ) && multi >= 0 && poisonous(mtmp->data) && !(mtmp->data->mlet == S_ZOMBIE)) {
		levl[mtmp->mx][mtmp->my].typ = POISONEDWELL;
		if (cansee(mtmp->mx, mtmp->my)) pline("%s poisons the well!", Monnam(mtmp));
    }

	/* but zombies will cure it... no matter how nonsensical this might seem, it's intentional */
    if (IS_POISONEDWELL(levl[mtmp->mx][mtmp->my].typ) && multi >= 0 && mtmp->data->mlet == S_ZOMBIE) {
		levl[mtmp->mx][mtmp->my].typ = WELL;
		if (cansee(mtmp->mx, mtmp->my)) pline("%s cures the well of its poison!", Monnam(mtmp));
    }

    if (IS_TOILET(levl[mtmp->mx][mtmp->my].typ) && multi >= 0 && flags.soundok && mtmp->mcanmove && ((distu(mtmp->mx,mtmp->my) <= BOLT_LIM*BOLT_LIM) || FemaleTrapKatharina) && ((mtmp->data->msound == MS_FART_QUIET) || (mtmp->data->msound == MS_FART_NORMAL) || (mtmp->data->msound == MS_FART_LOUD) || FemaleTrapThai ) ) {
	if (cansee(mtmp->mx,mtmp->my)) {
		pline("%s produces %s crapping noises with %s %s butt.", Monnam(mtmp), mtmp->data->msound == MS_FART_QUIET ? "tender" : mtmp->data->msound == MS_FART_NORMAL ? "beautiful" : "disgusting", mhis(mtmp), mtmp->female ? "sexy" : "ugly" );
	} else {
		You_hear("%s crapping noises.", mtmp->data->msound == MS_FART_QUIET ? "tender" : mtmp->data->msound == MS_FART_NORMAL ? "beautiful" : "disgusting");
	}
	if (mtmp->data->msound == MS_FART_QUIET) pline("Because of the wonderfully soft noises, you briefly forget what you were doing and just stand there.");
	else if (mtmp->data->msound == MS_FART_NORMAL) pline("You just can't believe that someone could produce such erotic noises, and are immobilized by your feelings.");
	else pline("This is really disgusting. You resist the urge to vomit, but fail to pay attention to your surroundings for a moment...");
      if (mtmp->data->msound == MS_FART_QUIET) nomul(-rnz(4 + mtmp->crapbonus), "listening to tender crapping noises");
      else if (mtmp->data->msound == MS_FART_NORMAL) nomul(-rnz(5 + mtmp->crapbonus), "listening to beautiful crapping noises");
      else nomul(-rnz(3 + mtmp->crapbonus), "listening to disgusting crapping noises");
      nomovemsg = "At last, you get yourself together, ready to move on.";

    }

    if (IS_TOILET(levl[mtmp->mx][mtmp->my].typ) && uarmc && uarmc->oartifact == ART_TOILET_NOISES && multi >= 0 && flags.soundok && mtmp->mcanmove && ((distu(mtmp->mx,mtmp->my) <= BOLT_LIM*BOLT_LIM) || FemaleTrapKatharina) && ((mtmp->data->msound != MS_FART_QUIET) && (mtmp->data->msound != MS_FART_NORMAL) && (mtmp->data->msound != MS_FART_LOUD) ) ) {
	if (cansee(mtmp->mx,mtmp->my)) {
		pline("%s produces crapping noises with %s %s butt.", Monnam(mtmp), mhis(mtmp), mtmp->female ? "sexy" : "ugly" );
	} else {
		You_hear("crapping noises.");
	}

	pline("Because you are stupid, you stop to listen.");
	nomul(-rnz(3 + mtmp->crapbonus), "listening to crapping noises");
      nomovemsg = "At last, you get yourself together, ready to move on.";
    }

    /* Gremlin multiplying won't go on forever since the hit points
     * keep going down, and when it gets to 1 hit point the clone
     * function will fail.
     */
    if (mtmp->data == &mons[PM_GREMLIN] && (inpool || infountain) && !rn2(5)) { /* lowered chance --Amy */
	if (split_mon(mtmp, (struct monst *)0))
	    dryup(mtmp->mx, mtmp->my, FALSE);
	if (inpool) water_damage(mtmp->minvent, FALSE, FALSE);
	return (0);
    } else if (mtmp->data == &mons[PM_IRON_GOLEM] && inpool && !rn2(5)) {
	int dam = d(2,6);
	if (cansee(mtmp->mx,mtmp->my))
	    pline("%s rusts.", Monnam(mtmp));
	mtmp->mhp -= dam;
	if (mtmp->mhpmax > dam) mtmp->mhpmax -= dam;
	if (mtmp->mhp < 1) {
	    mondead(mtmp);
	    if (mtmp->mhp < 1) return (1);
	}
	water_damage(mtmp->minvent, FALSE, FALSE);
	return (0);
    }

    if (inlava) {
	/*
	 * Lava effects much as water effects. Lava likers are able to
	 * protect their stuff. Fire resistant monsters can only protect
	 * themselves  --ALI
	 */
	if (!is_clinger(mtmp->data) && !(mtmp->data->mlet == S_FLYFISH) && !(mtmp->data == &mons[PM_UNMAGIC_FISH]) && !(mtmp->data == &mons[PM_ROXY_GRETA_S_SNEAKER]) && !likes_lava(mtmp->data)) {
	    if (!resists_fire(mtmp)) {
		if (cansee(mtmp->mx,mtmp->my))
		    pline("%s %s.", Monnam(mtmp),
			  mtmp->data == &mons[PM_WATER_ELEMENTAL] ?
			  "boils away" : "burns to a crisp");
		if (!rn2(20)) mondead(mtmp); /* give them a chance to survive --Amy */
		else (void) rloc(mtmp, FALSE); /* unrealistic, I know, but I want the game to be as hard as possible. */
		/* And monsters dying instantly is not my idea of hard. After all, you can instakill Croesus that way:
		 * freeze the moat, wait for him to step on it and burn the ice away, which is way too easy! */
	    }
	    else {
		if (--mtmp->mhp < 1) {
		    if (cansee(mtmp->mx,mtmp->my))
			pline("%s surrenders to the fire.", Monnam(mtmp));
			if (!rn2(20)) mondead(mtmp);
			else (void) rloc(mtmp, FALSE);
		}
		else if (cansee(mtmp->mx,mtmp->my))
		    pline("%s burns slightly.", Monnam(mtmp));
	    }
	    if (mtmp->mhp > 0) {
		(void) fire_damage(mtmp->minvent, FALSE, FALSE,
						mtmp->mx, mtmp->my);
		(void) rloc(mtmp, FALSE);
		return 0;
	    }
	    return (1);
	}
    } else if (inpool) {
	/* Most monsters drown in pools.  flooreffects() will take care of
	 * water damage to dead monsters' inventory, but survivors need to
	 * be handled here.  Swimmers are able to protect their stuff...
	 */
	if (!is_clinger(mtmp->data)
	    && !is_swimmer(mtmp->data) && !(mtmp->data->mlet == S_FLYFISH) && !(mtmp->data->mlet == S_EEL) && !mtmp->egotype_watersplasher && !amphibious(mtmp->data)) {
	    if (cansee(mtmp->mx,mtmp->my)) {
		    pline("%s drowns.", Monnam(mtmp));
	    }
	    if (u.ustuck && u.uswallow && u.ustuck == mtmp) {
	    /* This can happen after a purple worm plucks you off a
		flying steed while you are over water. */
		pline("%s sinks as water rushes in and flushes you out.",
			Monnam(mtmp));
	    }
		if (!rn2(20)) mondead(mtmp); /* see above for the reason why monsters survive here --Amy */
		else (void) rloc(mtmp, FALSE);
	    if (mtmp->mhp > 0) {
		(void) rloc(mtmp, FALSE);
		water_damage(mtmp->minvent, FALSE, FALSE);
		return 0;
	    }
	    return (1);
	}
    } /*else {*/
	/* but eels have a difficult time outside */
	if (mtmp->data->mlet == S_EEL && issoviet && !Is_waterlevel(&u.uz)) {

	/* "S_EEL monsters are horribly weak on land again." In Soviet Russia, people got kind of annoyed that they couldn't punch out kraken with a single hit of their bare fists, even though being capable of doing so makes the game way too easy. Other races though, will have to contend with eels in fair combat, water or no water. Who the heck decided to skew the odds in favor of the player anyway? Isn't Nethack supposed to be a difficult game??? --Amy */

	    if(mtmp->mhp > 1) mtmp->mhp--;
	    monflee(mtmp, 2, FALSE, FALSE);
	}

	/* commented out - would just make the game easier, and we don't want that --Amy */
    /*}*/
    return (0);
}

int
mcalcmove(mon)
struct monst *mon;
{
    int mmove = mon->data->mmove;

	if (mon->data == &mons[PM_HEFFER] || mon->data == &mons[PM_THE_SMITH] ) {
	/* Hephaisto, also known as "The Armorer", speeds up whenever he takes damage. --Amy */
		mmove *= mon->mhpmax;
		mmove /= mon->mhp;
	}

    if (mmove == 0) mmove++;	/* allowing stationary monsters to occasionally get turns --Amy */

    /* Note: MSLOW's `+ 1' prevents slowed speed 1 getting reduced to 0;
     *	     MFAST's `+ 2' prevents hasted speed 1 from becoming a no-op;
     *	     both adjustments have negligible effect on higher speeds.
     */
    if (mon->mspeed == MSLOW)
	mmove = (2 * mmove + 1) / 3;
    else if (mon->mspeed == MFAST)
	mmove = (4 * mmove + 2) / 3;

    if (mon == u.usteed) {
	if (u.ugallop && flags.mv) {
	    /* average movement is 1.50 times normal */
	    mmove = ((rn2(2) ? 4 : 5) * mmove) / 3;
	}
    }

    if (mon->egotype_speedster) mmove += 6;
    if (mon->egotype_racer) mmove += 12;

	if (is_highway(mon->mx, mon->my)) mmove += rnd(mmove);

	if ((MonsterSpeedBug || u.uprops[MONSTER_SPEED_BUG].extrinsic || have_monsterspeedstone()) && !rn2(2) && (mmove > 0)) {
		mmove *= 3;
		if (mmove == 3) mmove = 4;
		mmove /= 2;
	}

    if (mmove && uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "greek cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "grecheskiy plashch") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "yunon plash") ) ) mmove += 1;

	if (uarmh && mmove && OBJ_DESCR(objects[uarmh->otyp]) && ( !strcmp(OBJ_DESCR(objects[uarmh->otyp]), "formula one helmet") || !strcmp(OBJ_DESCR(objects[uarmh->otyp]), "formula odin shlem") || !strcmp(OBJ_DESCR(objects[uarmh->otyp]), "formula bir zarbdan") ) ) mmove += 1;

    if (uarmg && uarmg->oartifact == ART_AFK_MEANS_ASS_FUCKER && (dmgtype(mon->data, AD_SITM) || dmgtype(mon->data, AD_SEDU) || dmgtype(mon->data, AD_SSEX) ) ) mmove += 12;

    if (Race_if(PM_PLAYER_UNICORN)) mmove += 3; /* because you have double speed --Amy */

    return mmove;
}

/* actions that happen once per ``turn'', regardless of each
   individual monster's metabolism; some of these might need to
   be reclassified to occur more in proportion with movement rate */
void
mcalcdistress()
{
    struct monst *mtmp;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
	if (DEADMONSTER(mtmp)) continue;

	/* must check non-moving monsters once/turn in case
	 * they managed to end up in liquid */
	if (mtmp->data->mmove == 0) {
	    if (vision_full_recalc) vision_recalc(0);
	    if (minliquid(mtmp)) continue;
	}

	/* regenerate hit points */
	mon_regen(mtmp, FALSE);

	/* possibly polymorph shapechangers and lycanthropes */
	if (mtmp->cham && !rn2(6) && !Protection_from_shape_changers)
	    (void) mon_spec_poly(mtmp, (struct permonst *)0, 0L, FALSE,
		    cansee(mtmp->mx,mtmp->my) && flags.verbose, FALSE, FALSE);
	were_change(mtmp);

	/* gradually time out temporary problems */
	if (mtmp->mblinded && !--mtmp->mblinded)
	    mtmp->mcansee = 1;
	if (mtmp->mfrozen && !--mtmp->mfrozen)
	    mtmp->mcanmove = 1;
	if (mtmp->mfleetim && !--mtmp->mfleetim)
	    mtmp->mflee = 0;

	/* FIXME: mtmp->mlstmv ought to be updated here */
    }
}

int
movemon()
{
    register struct monst *mtmp, *nmtmp;
    register boolean somebody_can_move = FALSE;

#if 0
    /* part of the original warning code which was replaced in 3.3.1 */
    warnlevel = 0;
#endif

    /*
    Some of you may remember the former assertion here that
    because of deaths and other actions, a simple one-pass
    algorithm wasn't possible for movemon.  Deaths are no longer
    removed to the separate list fdmon; they are simply left in
    the chain with hit points <= 0, to be cleaned up at the end
    of the pass.

    The only other actions which cause monsters to be removed from
    the chain are level migrations and losedogs().  I believe losedogs()
    is a cleanup routine not associated with monster movements, and
    monsters can only affect level migrations on themselves, not others
    (hence the fetching of nmon before moving the monster).  Currently,
    monsters can jump into traps, read cursed scrolls of teleportation,
    and drink cursed potions of raise level to change levels.  These are
    all reflexive at this point.  Should one monster be able to level
    teleport another, this scheme would have problems.
    */

    for(mtmp = fmon; mtmp; mtmp = nmtmp) {
	nmtmp = mtmp->nmon;

	/* Find a monster that we have not treated yet.	 */
	if(DEADMONSTER(mtmp))
	    continue;
	if(mtmp->movement < NORMAL_SPEED)
	    continue;

	mtmp->movement -= NORMAL_SPEED;
	if (mtmp->movement >= NORMAL_SPEED)
	    somebody_can_move = TRUE;

	if (vision_full_recalc) vision_recalc(0);	/* vision! */

	if (minliquid(mtmp)) continue;

	/* if permamimics are ever not cloaked, e.g. because they got created by another monster polymorphing,
	 * cloak them again. --Amy */
	if ((permamimic(mtmp->data) || mtmp->egotype_permamimic ) && !mtmp->m_ap_type ) {
		set_mimic_sym(mtmp);
	}

	if (is_hider(mtmp->data) || mtmp->egotype_hide || mtmp->egotype_mimic) {
	    /* unwatched mimics and piercers may hide again  [MRS] */
	    if(restrap(mtmp))   continue;
	    if(mtmp->m_ap_type == M_AP_FURNITURE ||
				mtmp->m_ap_type == M_AP_OBJECT)
		    continue;
	    if(mtmp->mundetected) continue;
	}

	/* continue if the monster died fighting */
	if (Conflict && !mtmp->iswiz && mtmp->mcansee && haseyes(mtmp->data) ) {
	    /* Note:
	     *  Conflict does not take effect in the first round.
	     *  Therefore, A monster when stepping into the area will
	     *  get to swing at you.
	     *
	     *  The call to fightm() must be _last_.  The monster might
	     *  have died if it returns 1.
	     */
		/* Amy addition: monsters without eyes are now immune, because the Astral Plane is easy enough already! */
	    if (couldsee(mtmp->mx,mtmp->my) &&
		(distu(mtmp->mx,mtmp->my) <= /*BOLT_LIM*BOLT_LIM*/ 4) &&
							fightm(mtmp))
		continue;	/* mon might have died */
	}
	if(dochugw(mtmp))		/* otherwise just move the monster */
	    continue;
    }
#if 0
    /* part of the original warning code which was replaced in 3.3.1 */
    if(warnlevel > 0)
	warn_effects();
#endif
    if (any_light_source())
	vision_full_recalc = 1;	/* in case a mon moved with a light source */
    dmonsfree();	/* remove all dead monsters */

    /* a monster may have levteleported player -dlc */
    if (u.utotype) {
	deferred_goto();
	/* changed levels, so these monsters are dormant */
	somebody_can_move = FALSE;
    }

    return somebody_can_move;
}

#endif /* OVL1 */
#ifdef OVLB

#define mstoning(obj)	(ofood(obj) && \
					(touch_petrifies(&mons[(obj)->corpsenm]) || \
					(obj)->corpsenm == PM_MEDUSA))

/*
 * Maybe eat a metallic object (not just gold).
 * Return value: 0 => nothing happened, 1 => monster ate something,
 * 2 => monster died (it must have grown into a genocided form, but
 * that can't happen at present because nothing which eats objects
 * has young and old forms).
 */
int
meatmetal(mtmp)
	register struct monst *mtmp;
{
	register struct obj *otmp;
	struct permonst *ptr;
	int poly, grow, heal, mstone;

	/* If a pet, eating is handled separately, in dog.c */
	if (mtmp->mtame) return 0;

	/* Eats topmost metal object if it is there */
	for (otmp = level.objects[mtmp->mx][mtmp->my];
						otmp; otmp = otmp->nexthere) {
	    if (mtmp->data == &mons[PM_RUST_MONSTER] && !is_rustprone(otmp))
		continue;
	    if (is_metallic(otmp) && !obj_resists(otmp, 5, 95) &&
		touch_artifact(otmp,mtmp)) {
		if (mtmp->data == &mons[PM_RUST_MONSTER] && otmp->oerodeproof) {
		    if (cansee(mtmp->mx,mtmp->my) && flags.verbose) {
			pline("%s eats %s!",
				Monnam(mtmp),
				distant_name(otmp,doname));
		    }
		    /* The object's rustproofing is gone now */
		    otmp->oerodeproof = 0;
		    mtmp->mstun = 1;
		    if (canseemon(mtmp) && flags.verbose) {
			pline("%s spits %s out in disgust!",
			      Monnam(mtmp), distant_name(otmp,doname));
		    }
		/* KMH -- Don't eat indigestible/choking objects */
		} else if (otmp->otyp != AMULET_OF_STRANGULATION &&
				otmp->otyp != RIN_SLOW_DIGESTION) {
		    if (cansee(mtmp->mx,mtmp->my) && flags.verbose)
			pline("%s eats %s!", Monnam(mtmp),
				distant_name(otmp,doname));
		    else if (flags.soundok && flags.verbose) {
			You_hear("a crunching sound.");
			if (PlayerHearsSoundEffects) pline(issoviet ? "Ochen' tsennyy element metalla tol'ko chto poyel i vy budete pinat' sebya, yesli ya skazhu vam, chto eto bylo." : "Gruum!");
			}
		    mtmp->meating = otmp->owt/2 + 1;
		    if (mtmp->meating > 10) mtmp->meating = 10; /* arbitrary --Amy */
		    /* Heal up to the object's weight in hp */
		    if (mtmp->mhp < mtmp->mhpmax) {
			mtmp->mhp += objects[otmp->otyp].oc_weight;
			if (mtmp->mhp > mtmp->mhpmax) mtmp->mhp = mtmp->mhpmax;
		    }
		    if(otmp == uball) {
			unpunish();
			delobj(otmp);
		    } else if (otmp == uchain) {
			unpunish();	/* frees uchain */
		    } else {
			poly = polyfodder(otmp);
			grow = mlevelgain(otmp);
			heal = mhealup(otmp);
			mstone = mstoning(otmp);
			delobj(otmp);
			ptr = mtmp->data;
			if (poly) {
			    if (mon_spec_poly(mtmp,
				    (struct permonst *)0, 0L, FALSE,
				    cansee(mtmp->mx,mtmp->my) && flags.verbose,
				    FALSE, FALSE))
				ptr = mtmp->data;
			} else if (grow) {
			    ptr = grow_up(mtmp, (struct monst *)0);
			} else if (mstone && !rn2(4) ) {
			    if (poly_when_stoned(ptr)) {
				mon_to_stone(mtmp);
				ptr = mtmp->data;
			    } else if (!resists_ston(mtmp)) {
				if (canseemon(mtmp))
				    pline("%s turns to stone!", Monnam(mtmp));
				monstone(mtmp);
				ptr = (struct permonst *)0;
			    }
			} else if (heal) {
			    mtmp->mhp = mtmp->mhpmax;
			}
			if (!ptr) return 2;		 /* it died */
		    }
		    /* Left behind a pile? */
		    if (rnd(25) < 3)
			(void) mksobj_at(ROCK, mtmp->mx, mtmp->my, TRUE, FALSE);
		    newsym(mtmp->mx, mtmp->my);
		    return 1;
		}
	    }
	}
	return 0;
}

/*
 * Maybe eat a lithic object.
 * Return value: 0 => nothing happened, 1 => monster ate something,
 * 2 => monster died (it must have grown into a genocided form, but
 * that can't happen at present because nothing which eats objects
 * has young and old forms).
 */
int
meatlithic(mtmp)
	register struct monst *mtmp;
{
	register struct obj *otmp;
	struct permonst *ptr;
	int poly, grow, heal, mstone;

	/* If a pet, eating is handled separately, in dog.c */
	if (mtmp->mtame) return 0;

	/* Eats topmost lithic object if it is there */
	for (otmp = level.objects[mtmp->mx][mtmp->my];
						otmp; otmp = otmp->nexthere) { 

	    if (is_lithic(otmp) && !obj_resists(otmp, 5, 95) &&
		touch_artifact(otmp,mtmp)) {
			if (otmp->otyp != AMULET_OF_STRANGULATION &&
				otmp->otyp != RIN_SLOW_DIGESTION) {
		    if (cansee(mtmp->mx,mtmp->my) && flags.verbose)
			pline("%s eats %s!", Monnam(mtmp),
				distant_name(otmp,doname));
		    else if (flags.soundok && flags.verbose) {
			You_hear("a grating sound.");
			if (PlayerHearsSoundEffects) pline(issoviet ? "Luchshe nadeyat'sya, chto etot punkt kamennyy ne bylo chem-to vazhnym, potomu chto teper' poteryana navsegda." : "Wuoeing!");
			}
		    mtmp->meating = otmp->owt/2 + 1;
		    if (mtmp->meating > 10) mtmp->meating = 10; /* arbitrary --Amy */
		    /* Heal up to the object's weight in hp */
		    if (mtmp->mhp < mtmp->mhpmax) {
			mtmp->mhp += objects[otmp->otyp].oc_weight;
			if (mtmp->mhp > mtmp->mhpmax) mtmp->mhp = mtmp->mhpmax;
		    }
		    if(otmp == uball) {
			unpunish();
			delobj(otmp);
		    } else if (otmp == uchain) {
			unpunish();	/* frees uchain */
		    } else {
			poly = polyfodder(otmp);
			grow = mlevelgain(otmp);
			heal = mhealup(otmp);
			mstone = mstoning(otmp);
			delobj(otmp);
			ptr = mtmp->data;
			if (poly) {
			    if (mon_spec_poly(mtmp,
				    (struct permonst *)0, 0L, FALSE,
				    cansee(mtmp->mx,mtmp->my) && flags.verbose,
				    FALSE, FALSE))
				ptr = mtmp->data;
			} else if (grow) {
			    ptr = grow_up(mtmp, (struct monst *)0);
			} else if (mstone && !rn2(4) ) {
			    if (poly_when_stoned(ptr)) {
				mon_to_stone(mtmp);
				ptr = mtmp->data;
			    } else if (!resists_ston(mtmp)) {
				if (canseemon(mtmp))
				    pline("%s turns to stone!", Monnam(mtmp));
				monstone(mtmp);
				ptr = (struct permonst *)0;
			    }
			} else if (heal) {
			    mtmp->mhp = mtmp->mhpmax;
			}
			if (!ptr) return 2;		 /* it died */
		    }
		    newsym(mtmp->mx, mtmp->my);
		    return 1;
		}
	    }
	}
	return 0;
}

int
meatanything(mtmp)
	register struct monst *mtmp;
{
	register struct obj *otmp;
	struct permonst *ptr;
	int poly, grow, heal, mstone;

	/* If a pet, eating is handled separately, in dog.c */
	if (mtmp->mtame) return 0;

	/* Eats topmost object if it is there */
	for (otmp = level.objects[mtmp->mx][mtmp->my];
						otmp; otmp = otmp->nexthere) {
	    if (!obj_resists(otmp, 5, 95) &&
		touch_artifact(otmp,mtmp)) {
		if (otmp->otyp != AMULET_OF_STRANGULATION &&
				otmp->otyp != RIN_SLOW_DIGESTION) {
		    if (cansee(mtmp->mx,mtmp->my) && flags.verbose)
			pline("%s eats %s!", Monnam(mtmp),
				distant_name(otmp,doname));
		    else if (flags.soundok && flags.verbose) {
			You_hear("a chewing sound.");
			if (PlayerHearsSoundEffects) pline(issoviet ? "Ochen' tsennyy element metalla tol'ko chto poyel i vy budete pinat' sebya, yesli ya skazhu vam, chto eto bylo." : "Gruum!");
			}
		    mtmp->meating = otmp->owt/2 + 1;
		    if (mtmp->meating > 10) mtmp->meating = 10; /* arbitrary --Amy */
		    /* Heal up to the object's weight in hp */
		    if (mtmp->mhp < mtmp->mhpmax) {
			mtmp->mhp += objects[otmp->otyp].oc_weight;
			if (mtmp->mhp > mtmp->mhpmax) mtmp->mhp = mtmp->mhpmax;
		    }
		    if(otmp == uball) {
			unpunish();
			delobj(otmp);
		    } else if (otmp == uchain) {
			unpunish();	/* frees uchain */
		    } else {
			poly = polyfodder(otmp);
			grow = mlevelgain(otmp);
			heal = mhealup(otmp);
			mstone = mstoning(otmp);
			delobj(otmp);
			ptr = mtmp->data;
			if (poly) {
			    if (mon_spec_poly(mtmp,
				    (struct permonst *)0, 0L, FALSE,
				    cansee(mtmp->mx,mtmp->my) && flags.verbose,
				    FALSE, FALSE))
				ptr = mtmp->data;
			} else if (grow) {
			    ptr = grow_up(mtmp, (struct monst *)0);
			} else if (mstone && !rn2(4) ) {
			    if (poly_when_stoned(ptr)) {
				mon_to_stone(mtmp);
				ptr = mtmp->data;
			    } else if (!resists_ston(mtmp)) {
				if (canseemon(mtmp))
				    pline("%s turns to stone!", Monnam(mtmp));
				monstone(mtmp);
				ptr = (struct permonst *)0;
			    }
			} else if (heal) {
			    mtmp->mhp = mtmp->mhpmax;
			}
			if (!ptr) return 2;		 /* it died */
		    }
		    newsym(mtmp->mx, mtmp->my);
		    return 1;
		}
	    }
	}
	return 0;
}

void
meatcorpse(mtmp)
	register struct monst *mtmp;
{
	register struct obj *otmp, *otmpB;
 
	/* If a pet, eating is handled separately, in dog.c */
	if (mtmp->mtame) return;
  
	/* Eats topmost corpse if it is there */
	for (otmp = level.objects[mtmp->mx][mtmp->my];
						    otmp; otmp = otmp->nexthere)
	       if (otmp->otyp == CORPSE &&
		  otmp->age+50 <= monstermoves) {
		    if (cansee(mtmp->mx,mtmp->my) && flags.verbose)
			pline("%s eats %s!", Monnam(mtmp),
				distant_name(otmp,doname));
		    else if (flags.soundok && flags.verbose) {
			You(Hallucination ? "hear an alien's noises!" : "hear an awful gobbling noise!");
			if (PlayerHearsSoundEffects) pline(issoviet ? "Vy boites' ne meneye, vy malen'kiy rebenok? Postaraytes', chtoby ne ispachkat' sebya, kha-kha!" : "Kwololololo lololololo!");
			}
		    mtmp->meating = 2;
		    delobj(otmp);

			if (mtmp->data == &mons[PM_CORPULENT_DOG] || mtmp->data == &mons[PM_THICK_POTATO] || mtmp->data == &mons[PM_BLACK_MUZZLE] || mtmp->data == &mons[PM_CORPSE_SPITTER] || mtmp->data == &mons[PM_MUZZLE_FIEND] || mtmp->data == &mons[PM_MAW_FIEND] || mtmp->data == &mons[PM_ROCKET_MUZZLE]) {

				otmpB = mksobj(ROCKET, TRUE, FALSE);
				if (otmpB) {
					otmpB->quan = 1;
					otmpB->owt = weight(otmpB);
					(void) mpickobj(mtmp, otmpB, FALSE);
					}
				}

			if (mtmp->data == &mons[PM_SPIT_DEMON]) {

				otmpB = mksobj(ROCK, TRUE, FALSE);
				if (otmpB) {
					otmpB->quan = 5;
					otmpB->owt = weight(otmpB);
					(void) mpickobj(mtmp, otmpB, FALSE);
				}

			}



		    break; /* only eat one at a time... */
		  }
      newsym(mtmp->mx, mtmp->my);
}
 
int
meatobj(mtmp)		/* for gelatinous cubes */
	register struct monst *mtmp;
{
	register struct obj *otmp, *otmp2;
	struct permonst *ptr;
	int poly, grow, heal, count = 0, ecount = 0;
	char buf[BUFSZ];

	buf[0] = '\0';
	/* If a pet, eating is handled separately, in dog.c */
	if (mtmp->mtame) return 0;

	/* Eats organic objects, including cloth and wood, if there */
	/* Engulfs others, except huge rocks and metal attached to player */
	for (otmp = level.objects[mtmp->mx][mtmp->my]; otmp; otmp = otmp2) {
	    otmp2 = otmp->nexthere;
	    if (is_organic(otmp) && !obj_resists(otmp, 5, 95) &&
		    touch_artifact(otmp,mtmp)) {
		if (otmp->otyp == CORPSE && touch_petrifies(&mons[otmp->corpsenm]) &&
			!resists_ston(mtmp))
		    continue;
		if (otmp->otyp == AMULET_OF_STRANGULATION ||
				otmp->otyp == RIN_SLOW_DIGESTION)
		    continue;
		++count;
		if (cansee(mtmp->mx,mtmp->my) && flags.verbose)
		    pline("%s eats %s!", Monnam(mtmp),
			    distant_name(otmp, doname));
		else if (flags.soundok && flags.verbose) {
		    You_hear("a slurping sound.");
			if (PlayerHearsSoundEffects) pline(issoviet ? "Skoreye vsego, eto bylo chto-to, chto vy, vozmozhno, khoteli ispol'zovat', ili, mozhet byt', dazhe vash taynik! Razve eto ne veselo?" : "Chllp!");
		}
		/* Heal up to the object's weight in hp */
		if (mtmp->mhp < mtmp->mhpmax) {
		    mtmp->mhp += objects[otmp->otyp].oc_weight;
		    if (mtmp->mhp > mtmp->mhpmax) mtmp->mhp = mtmp->mhpmax;
		}
		if (otmp->otyp == MEDICAL_KIT)
		    delete_contents(otmp);
		if (Has_contents(otmp)) {
		    register struct obj *otmp3;
		    /* contents of eaten containers become engulfed; this
		       is arbitrary, but otherwise g.cubes are too powerful */
		    while ((otmp3 = otmp->cobj) != 0) {
			obj_extract_self(otmp3);
			if ( (otmp->otyp == ICE_BOX || otmp->otyp == ICE_BOX_OF_HOLDING || otmp->otyp == ICE_BOX_OF_WATERPROOFING || otmp->otyp == ICE_BOX_OF_DIGESTION) && otmp3->otyp == CORPSE) {
			    otmp3->age = monstermoves - otmp3->age;
			    start_corpse_timeout(otmp3);
			}
			(void) mpickobj(mtmp, otmp3, FALSE);
		    }
		}
		poly = polyfodder(otmp);
		grow = mlevelgain(otmp);
		heal = mhealup(otmp);
		delobj(otmp);		/* munch */
		ptr = mtmp->data;
		if (poly) {
		    if (mon_spec_poly(mtmp, (struct permonst *)0, 0L, FALSE,
			    cansee(mtmp->mx,mtmp->my) && flags.verbose,
			    FALSE, FALSE))
			ptr = mtmp->data;
		} else if (grow) {
		    ptr = grow_up(mtmp, (struct monst *)0);
		} else if (heal) {
		    mtmp->mhp = mtmp->mhpmax;
		}
		/* in case it polymorphed or died */
		if (ptr != &mons[PM_GELATINOUS_CUBE])
		    return !ptr ? 2 : 1;
	    } else if (otmp->oclass != ROCK_CLASS &&
				    otmp != uball && otmp != uchain) {
		++ecount;
		if (ecount == 1) {
			sprintf(buf, "%s engulfs %s.", Monnam(mtmp),
			    distant_name(otmp,doname));
		} else if (ecount == 2)
			sprintf(buf, "%s engulfs several objects.", Monnam(mtmp));
		obj_extract_self(otmp);
		(void) mpickobj(mtmp, otmp, FALSE);	/* slurp */
	    }
	    /* Engulf & devour is instant, so don't set meating */
	    if (mtmp->minvis) newsym(mtmp->mx, mtmp->my);
	}
	if (ecount > 0) {
	    if (cansee(mtmp->mx, mtmp->my) && flags.verbose && buf[0])
		pline("%s", buf);
	    else if (flags.soundok && flags.verbose) {
	    	You_hear("%s slurping sound%s.",
			ecount == 1 ? "a" : "several",
			ecount == 1 ? "" : "s");
		if (PlayerHearsSoundEffects) pline(issoviet ? "Skoreye vsego, eto bylo chto-to, chto vy, vozmozhno, khoteli ispol'zovat', ili, mozhet byt', dazhe vash taynik! Razve eto ne veselo?" : "Chllp!");
		}
	}
	return ((count > 0) || (ecount > 0)) ? 1 : 0;
}

void
mpickgold(mtmp)
	register struct monst *mtmp;
{
    register struct obj *gold;
    int mat_idx;

    if ((gold = g_at(mtmp->mx, mtmp->my)) != 0) {
	mat_idx = objects[gold->otyp].oc_material;

	if (uchain && (gold == uchain) ) return;
	if (uchain && (gold == uball) ) return;

#ifndef GOLDOBJ
	mtmp->mgold += gold->quan;
	delobj(gold);
#else
        obj_extract_self(gold);
        add_to_minv(mtmp, gold);
#endif
	if (cansee(mtmp->mx, mtmp->my) ) {
	    if (flags.verbose && !mtmp->isgd)
		pline("%s picks up some %s.", Monnam(mtmp),
			mat_idx == GOLD ? "gold" : "money");
	    newsym(mtmp->mx, mtmp->my);
	}
    }
}
#endif /* OVLB */
#ifdef OVL2

boolean
mpickstuff(mtmp, str)
	register struct monst *mtmp;
	register const char *str;
{
	register struct obj *otmp, *otmp2;

/*	prevent shopkeepers from leaving the door of their shop */
	if(mtmp->isshk && inhishop(mtmp)) return FALSE;

	for(otmp = level.objects[mtmp->mx][mtmp->my]; otmp; otmp = otmp2) {
	    otmp2 = otmp->nexthere;
/*	Nymphs take everything.  Most monsters don't pick up corpses. */
	    if (!str ? searches_for_item(mtmp,otmp) :
		  !!(index(str, otmp->oclass))) {
		if (otmp->otyp == CORPSE && mtmp->data->mlet != S_NYMPH &&
			/* let a handful of corpse types thru to can_carry() */
			!touch_petrifies(&mons[otmp->corpsenm]) &&
			otmp->corpsenm != PM_LIZARD &&
			otmp->corpsenm != PM_CAVE_LIZARD &&
			otmp->corpsenm != PM_PREHISTORIC_CAVE_LIZARD &&
			otmp->corpsenm != PM_CHAOS_LIZARD &&
			otmp->corpsenm != PM_CHAOTIC_LIZARD &&
			otmp->corpsenm != PM_LIZARD_OF_YENDOR &&
			otmp->corpsenm != PM_GRASS_LIZARD &&
			otmp->corpsenm != PM_BLUE_LIZARD &&
			otmp->corpsenm != PM_SWAMP_LIZARD &&
			otmp->corpsenm != PM_SPITTING_LIZARD &&
			otmp->corpsenm != PM_LIZARD_EEL &&
			otmp->corpsenm != PM_LIZARD_MAN &&
			otmp->corpsenm != PM_LIZARD_KING &&
			otmp->corpsenm != PM_EEL_LIZARD &&
			otmp->corpsenm != PM_ANTI_STONE_LIZARD &&
			otmp->corpsenm != PM_HUGE_LIZARD &&
			otmp->corpsenm != PM_ROCK_LIZARD &&
			otmp->corpsenm != PM_BABY_CAVE_LIZARD &&
			otmp->corpsenm != PM_NIGHT_LIZARD &&
			otmp->corpsenm != PM_SAND_TIDE &&
			otmp->corpsenm != PM_FBI_AGENT &&
			otmp->corpsenm != PM_OWN_SMOKE &&
			otmp->corpsenm != PM_GRANDPA &&
			otmp->corpsenm != PM_HIDDEN_LIZARD &&
			otmp->corpsenm != PM_BLACK_LIZARD &&
			otmp->corpsenm != PM_DEFORMED_LIZARD &&
			otmp->corpsenm != PM_CLINGING_LIZARD &&
			otmp->corpsenm != PM_MIMIC_LIZARD &&
			otmp->corpsenm != PM_KARMIC_LIZARD &&
			otmp->corpsenm != PM_GREEN_LIZARD &&
			otmp->corpsenm != PM_MONSTER_LIZARD &&
			otmp->corpsenm != PM_ICE_LIZARD &&
			otmp->corpsenm != PM_FIRE_LIZARD &&
			otmp->corpsenm != PM_LIGHTNING_LIZARD &&
			otmp->corpsenm != PM_GIANT_LIZARD &&
			!acidic(&mons[otmp->corpsenm])) continue;
		if (!touch_artifact(otmp,mtmp)) continue;
		if (!can_carry(mtmp,otmp)) continue;
		if (is_waterypool(mtmp->mx,mtmp->my)) continue;
		if (is_watertunnel(mtmp->mx,mtmp->my)) continue;
		if ((otmp->oinvis && !perceives(mtmp->data) || otmp->oinvisreal) ) continue;
		if (cansee(mtmp->mx,mtmp->my) && flags.verbose)
			pline("%s picks up %s.", Monnam(mtmp),
			      (distu(mtmp->mx, mtmp->my) <= 5) ?
				doname(otmp) : distant_name(otmp, doname));
		obj_extract_self(otmp);
		/* unblock point after extract, before pickup */
		if (otmp->otyp == BOULDER)
		    unblock_point(otmp->ox,otmp->oy);	/* vision */
		(void) mpickobj(mtmp, otmp, FALSE);	/* may merge and free otmp */
		m_dowear(mtmp, FALSE);
		newsym(mtmp->mx, mtmp->my);
		return TRUE;			/* pick only one object */
	    }
	}
	return FALSE;
}

#endif /* OVL2 */
#ifdef OVL0

int
curr_mon_load(mtmp)
register struct monst *mtmp;
{
	register int curload = 0;
	register struct obj *obj;

	for(obj = mtmp->minvent; obj; obj = obj->nobj) {
		if(obj->otyp != BOULDER || !throws_rocks(mtmp->data))
			curload += obj->owt;
	}

	return curload;
}

int
max_mon_load(mtmp)
register struct monst *mtmp;
{
	register long maxload;

	/* Base monster carrying capacity is equal to human maximum
	 * carrying capacity, or half human maximum if not strong.
	 * (for a polymorphed player, the value used would be the
	 * non-polymorphed carrying capacity instead of max/half max).
	 * This is then modified by the ratio between the monster weights
	 * and human weights.  Corpseless monsters are given a capacity
	 * proportional to their size instead of weight.
	 */
	if (!mtmp->data->cwt)
		maxload = (MAX_CARR_CAP * (long)mtmp->data->msize) / MZ_HUMAN;
	else if (!strongmonst(mtmp->data)
		|| (strongmonst(mtmp->data) && (mtmp->data->cwt > WT_HUMAN)))
		maxload = (MAX_CARR_CAP * (long)mtmp->data->cwt) / WT_HUMAN;
	else	maxload = MAX_CARR_CAP; /*strong monsters w/cwt <= WT_HUMAN*/

	if (!strongmonst(mtmp->data)) maxload /= 2;

	if (maxload < 200) maxload = 200;

	return (int) maxload;
}

/* for restricting monsters' object-pickup */
boolean
can_carry(mtmp,otmp)
struct monst *mtmp;
struct obj *otmp;
{
	int otyp = otmp->otyp, newload = otmp->owt;
	struct permonst *mdat = mtmp->data;

	if (notake(mdat)) return FALSE;		/* can't carry anything */

	if (otyp == CORPSE && touch_petrifies(&mons[otmp->corpsenm]) &&
		!(mtmp->misc_worn_check & W_ARMG) && !resists_ston(mtmp))
	    return FALSE;
	if (otyp == CORPSE && is_rider(&mons[otmp->corpsenm]))
	    return FALSE;
	if (otyp == CORPSE && is_deadlysin(&mons[otmp->corpsenm]))
	    return FALSE;
	if (objects[otyp].oc_material == SILVER && hates_silver(mdat) &&
		(otyp != BELL_OF_OPENING || !is_covetous(mdat)))
	    return FALSE;

	/* Steeds don't pick up stuff (to avoid shop abuse) */
	if (mtmp == u.usteed) return (FALSE);
	if (mtmp->isshk) return(TRUE); /* no limit */
	if (mtmp->mpeaceful && !mtmp->mtame) return(FALSE);
	/* otherwise players might find themselves obligated to violate
	 * their alignment if the monster takes something they need
	 */

	/* special--boulder throwers carry unlimited amounts of boulders */
	if (throws_rocks(mdat) && otyp == BOULDER)
		return(TRUE);

	/* nymphs deal in stolen merchandise, but not boulders or statues */
	if (mdat->mlet == S_NYMPH)
		return (boolean)(otmp->oclass != ROCK_CLASS);

	if (curr_mon_load(mtmp) + newload > max_mon_load(mtmp)) return FALSE;

	/* if the monster hates silver,  don't pick it up */
	if (objects[otmp->otyp].oc_material == SILVER && hates_silver(mtmp->data)) 
		return(FALSE);

	if(curr_mon_load(mtmp) + newload > max_mon_load(mtmp)) return(FALSE);

	return(TRUE);
}

/* return number of acceptable neighbour positions */
int
mfndpos(mon, poss, info, flag)
	register struct monst *mon;
	coord *poss;	/* coord poss[9] */
	long *info;	/* long info[9] */
	long flag;
{
	struct permonst *mdat = mon->data;
	register xchar x,y,nx,ny;
	register int cnt = 0;
	register uchar ntyp;
	uchar nowtyp;
	boolean wantpool,poolok,wantlava,lavaok,nodiag;
	boolean rockok = FALSE, treeok = FALSE, thrudoor;
	int maxx, maxy;

	x = mon->mx;
	y = mon->my;
	nowtyp = levl[x][y].typ;

	nodiag = (mdat == &mons[PM_GRID_BUG] || mdat == &mons[PM_WEREGRIDBUG] || mdat == &mons[PM_GRID_XORN] || mdat == &mons[PM_STONE_BUG] || mdat == &mons[PM_WEAPON_BUG] || mdat == &mons[PM_SPECIFIC_BUG] || mdat == &mons[PM_CONTAMINATED_BUG] || mdat == &mons[PM_NATURAL_BUG] || mdat == &mons[PM_MELEE_BUG] || mdat == &mons[PM_VORPAL_GRID_BUG] || (uarmf && !rn2(10) && OBJ_DESCR(objects[uarmf->otyp]) && ( !strcmp(OBJ_DESCR(objects[uarmf->otyp]), "chess boots") || !strcmp(OBJ_DESCR(objects[uarmf->otyp]), "shakhmatnyye sapogi") || !strcmp(OBJ_DESCR(objects[uarmf->otyp]), "shaxmat chizilmasin") ) ) );
	wantpool = mdat->mlet == S_EEL || mdat->mlet == S_FLYFISH || mdat == &mons[PM_HUMAN_WEREPIRANHA] || mdat == &mons[PM_HUMAN_WEREEEL] || mdat == &mons[PM_HUMAN_WEREKRAKEN] || mdat == &mons[PM_HUMAN_WEREFLYFISH] || mdat == &mons[PM_CONCORDE__] || mdat == &mons[PM_SWIMMER_TROLL] || mdat == &mons[PM_MISTER_SUBMARINE] || mdat == &mons[PM_EEL_GOLEM] || mdat == &mons[PM_WATER_TURRET] || mdat == &mons[PM_AQUA_TURRET] || mdat == &mons[PM_DIVER_TROLL] || mdat == &mons[PM_PUNT] || mdat == &mons[PM_LUXURY_YACHT] || mdat == &mons[PM_SUBMARINE_GOBLIN] ;
	poolok = (is_flyer(mdat) || mon->egotype_flying || is_clinger(mdat) || mon->egotype_watersplasher ||
		 (is_swimmer(mdat) && !wantpool)) && !(mdat->mlet == S_FLYFISH || mdat == &mons[PM_HUMAN_WEREFLYFISH] || mdat == &mons[PM_CONCORDE__]);
	wantlava = (mdat->mlet == S_FLYFISH || mdat == &mons[PM_HUMAN_WEREFLYFISH] || mdat == &mons[PM_CONCORDE__]);
	lavaok = is_flyer(mdat) || mon->egotype_flying || is_clinger(mdat) || (likes_lava(mdat) && !wantlava);
	thrudoor = ((flag & (ALLOW_WALL|BUSTDOOR)) != 0L);
	treeok = is_flyer(mdat) || mon->egotype_flying; /* flying monsters, but not flying players, can pass over trees --Amy */
	if (flag & ALLOW_DIG) {
	    struct obj *mw_tmp;

	    /* need to be specific about what can currently be dug */
	    if (!needspick(mdat)) {
		rockok = treeok = TRUE;
	    } else if ((mw_tmp = MON_WEP(mon)) && mw_tmp->cursed &&
		       mon->weapon_check == NO_WEAPON_WANTED) {
		rockok = is_pick(mw_tmp);
	    } else {
		rockok = (m_carrying(mon, PICK_AXE) || m_carrying(mon, CONGLOMERATE_PICK) || m_carrying(mon, BRONZE_PICK) ||
			  (m_carrying(mon, DWARVISH_MATTOCK) &&
			   !which_armor(mon, W_ARMS)) || 
			  (m_carrying(mon, SOFT_MATTOCK) &&
			   !which_armor(mon, W_ARMS)) );
		treeok = (m_carrying(mon, AXE) || m_carrying(mon, OBSIDIAN_AXE) || m_carrying(mon, SPIRIT_AXE) || m_carrying(mon, TUBING_PLIERS) ||
			  (m_carrying(mon, BATTLE_AXE) &&
			   !which_armor(mon, W_ARMS)) ||
			  (m_carrying(mon, MOON_AXE) &&
			   !which_armor(mon, W_ARMS)) ||
			  (m_carrying(mon, CHEMISTRY_SPACE_AXE) &&
			   !which_armor(mon, W_ARMS)) ||
			  (m_carrying(mon, DWARVISH_BATTLE_AXE) &&
			   !which_armor(mon, W_ARMS)));
	    }
	    thrudoor |= rockok || treeok;
	}
	if (is_flyer(mdat) || mon->egotype_flying) treeok = TRUE; /* fail safe */

nexttry:	/* eels prefer the water, but if there is no water nearby,
		   they will crawl over land */
	if(mon->mconf) {
		flag |= ALLOW_ALL;
		flag &= ~NOTONL;
	}
	if(!mon->mcansee)
		flag |= ALLOW_SSM;
	maxx = min(x+1,COLNO-1);
	maxy = min(y+1,ROWNO-1);
	for(nx = max(1,x-1); nx <= maxx; nx++)
	  for(ny = max(0,y-1); ny <= maxy; ny++) {
	    if(nx == x && ny == y) continue;
	    if(IS_ROCK(ntyp = levl[nx][ny].typ) && ntyp != FARMLAND &&
		 !(ntyp == TUNNELWALL && !is_flyer(mdat) && !is_floater(mdat) && !(mon->egotype_flying)) &&
		 !(ntyp == MOUNTAIN && (is_flyer(mdat) || is_floater(mdat) || (mon->egotype_flying))) &&
	       !((flag & ALLOW_WALL) && may_passwall(nx,ny)) &&
	       !((IS_TREE(ntyp) ? treeok : rockok) && may_dig(nx,ny))) continue;
	    /* KMH -- Added iron bars */
	    if (ntyp == IRONBARS && !(flag & ALLOW_BARS)) continue;
	    /* ALI -- Artifact doors (no passage unless open/openable) */
	    if (IS_DOOR(ntyp) && (rn2( (!nolimbs(mdat) && !slithy(mdat) && mdat->msize >= MZ_HUMAN ) ? 50 : 200) || artifact_door(nx, ny) ) ) 	/* let's allow monsters to bypass locked doors sometimes. --Amy */
	/* with evil patch idea by jonadab that allows human-or-larger-sized monsters with legs to break them more easily */

		if (artifact_door(nx, ny) ?
		    (levl[nx][ny].doormask & D_CLOSED) && !(flag & OPENDOOR)
		      || levl[nx][ny].doormask & D_LOCKED :
		    !amorphous(mdat) &&
	       ((levl[nx][ny].doormask & D_CLOSED && !(flag & OPENDOOR)) ||
		(levl[nx][ny].doormask & D_LOCKED && !(flag & UNLOCKDOOR))) &&
	       !thrudoor) continue;
	    if(nx != x && ny != y && (nodiag ||
#ifdef REINCARNATION
	       ((IS_DOOR(nowtyp) &&
		 ((levl[x][y].doormask & ~D_BROKEN) || Is_rogue_level(&u.uz))) ||
		(IS_DOOR(ntyp) &&
		 ((levl[nx][ny].doormask & ~D_BROKEN) || Is_rogue_level(&u.uz))))
#else
	       ((IS_DOOR(nowtyp) && (levl[x][y].doormask & ~D_BROKEN)) ||
		(IS_DOOR(ntyp) && (levl[nx][ny].doormask & ~D_BROKEN)))
#endif
	       ))
		continue;
	    /*if((is_pool(nx,ny) == wantpool || is_lava(nx,ny) == wantlava || poolok) &&
	       ( lavaok || wantlava || !is_lava(nx,ny))) {*/

	    if(( ( ( (is_waterypool(nx,ny) && !(is_crystalwater(nx,ny))) || is_watertunnel(nx,ny)) == wantpool) ||
		 (is_crystalwater(nx,ny) && wantpool) || (is_shiftingsand(nx,ny) && wantpool) ||
		 (is_lava(nx,ny) && wantlava) || (is_styxriver(nx,ny) && wantlava) || poolok) &&
	       (lavaok || wantlava || (!is_lava(nx,ny) && !is_styxriver(nx,ny)) )) {
		int dispx, dispy;
		boolean monseeu = (mon->mcansee && (!Invis || perceives(mdat)));
		boolean checkobj = OBJ_AT(nx,ny);

		/* Displacement also displaces the Elbereth/scare monster,
		 * as long as you are visible.
		 */
		if(Displaced && monseeu && (mon->mux==nx) && (mon->muy==ny)) {
		    dispx = u.ux;
		    dispy = u.uy;
		} else {
		    dispx = nx;
		    dispy = ny;
		}

		info[cnt] = 0;
		if ((checkobj || Displaced) && onscary(dispx, dispy, mon)) {
		    if(!(flag & ALLOW_SSM) && !rn2(3)) continue;
		    info[cnt] |= ALLOW_SSM;
		}
		if((nx == u.ux && ny == u.uy) ||
		   (nx == mon->mux && ny == mon->muy)) {
			if (nx == u.ux && ny == u.uy) {
				/* If it's right next to you, it found you,
				 * displaced or no.  We must set mux and muy
				 * right now, so when we return we can tell
				 * that the ALLOW_U means to attack _you_ and
				 * not the image.
				 */
				mon->mux = u.ux;
				mon->muy = u.uy;
			}
			if(!(flag & ALLOW_U)) continue;
			info[cnt] |= ALLOW_U;
		} else {
			if(MON_AT(nx, ny)) {
				struct monst *mtmp2 = m_at(nx, ny);
				long mmflag = flag | mm_aggression(mon, mtmp2);

				if (!(mmflag & ALLOW_M)) continue;
				info[cnt] |= ALLOW_M;
				if (mtmp2->mtame) {
					if (!(mmflag & ALLOW_TM)) continue;
					info[cnt] |= ALLOW_TM;
				}
			}
			/* Note: ALLOW_SANCT only prevents movement, not */
			/* attack, into a temple. */
			if(level.flags.has_temple &&
			   *in_rooms(nx, ny, TEMPLE) &&
			   !*in_rooms(x, y, TEMPLE) &&
			   in_your_sanctuary((struct monst *)0, nx, ny)) {
				if(!(flag & ALLOW_SANCT) && !rn2(5) ) continue;
				info[cnt] |= ALLOW_SANCT;
			}
		}
		if(checkobj && sobj_at(CLOVE_OF_GARLIC, nx, ny)) {
			if(flag & NOGARLIC) continue;
			info[cnt] |= NOGARLIC;
		}
		if(checkobj && sobj_at(BOULDER, nx, ny)) {
			if(!(flag & ALLOW_ROCK)) continue;
			info[cnt] |= ALLOW_ROCK;
		}
		if (monseeu && onlineu(nx,ny)) {
			if(flag & NOTONL) continue;
			info[cnt] |= NOTONL;
		}
		if (nx != x && ny != y && bad_rock(mon, x, ny)
			    && bad_rock(mon, nx, y)
			    && (bigmonst(mdat) || (curr_mon_load(mon) > 5000)))
			continue;
		/* The monster avoids a particular type of trap if it's familiar
		 * with the trap type.  Pets get ALLOW_TRAPS and checking is
		 * done in dogmove.c.  In either case, "harmless" traps are
		 * neither avoided nor marked in info[].
		 */
		{ register struct trap *ttmp = t_at(nx, ny);
		    if(ttmp) {
			if(ttmp->ttyp >= TRAPNUM || ttmp->ttyp == 0)  {
impossible("A monster looked at a very strange trap of type %d.", ttmp->ttyp);
			    continue;
			}
			if ((ttmp->ttyp != RUST_TRAP
					|| mdat == &mons[PM_FLAMING_SPHERE]
					|| mdat == &mons[PM_IRON_GOLEM])
				&& ttmp->ttyp != STATUE_TRAP
				&& ttmp->ttyp != SHIT_TRAP
				&& ttmp->ttyp != ANIMATION_TRAP
				&& ttmp->ttyp != RMB_LOSS_TRAP
				&& ttmp->ttyp != SUPERSCROLLER_TRAP
				&& ttmp->ttyp != ACTIVE_SUPERSCROLLER_TRAP
				&& ttmp->ttyp != SPEED_TRAP
				&& ttmp->ttyp != MENU_TRAP
				&& ttmp->ttyp != SWARM_TRAP
				&& ttmp->ttyp != AUTOMATIC_SWITCHER
				&& ttmp->ttyp != HEEL_TRAP
				&& ttmp->ttyp != VULN_TRAP
				&& ttmp->ttyp != DISPLAY_TRAP
				&& ttmp->ttyp != SPELL_LOSS_TRAP
				&& ttmp->ttyp != YELLOW_SPELL_TRAP
				&& ttmp->ttyp != AUTO_DESTRUCT_TRAP
				&& ttmp->ttyp != MEMORY_TRAP
				&& ttmp->ttyp != INVENTORY_TRAP
				&& ttmp->ttyp != BLACK_NG_WALL_TRAP
				&& ttmp->ttyp != UNKNOWN_TRAP
				&& ttmp->ttyp != TRAP_PERCENTS
				&& ttmp->ttyp != LAVA_TRAP
				&& ttmp->ttyp != FLOOD_TRAP
				&& ttmp->ttyp != FREE_HAND_TRAP
				&& ttmp->ttyp != UNIDENTIFY_TRAP
				&& ttmp->ttyp != THIRST_TRAP
				&& ttmp->ttyp != LUCK_TRAP
				&& ttmp->ttyp != SHADES_OF_GREY_TRAP
				&& ttmp->ttyp != ITEM_TELEP_TRAP
				&& ttmp->ttyp != GRAVITY_TRAP
				&& ttmp->ttyp != STONE_TO_FLESH_TRAP
				&& ttmp->ttyp != QUICKSAND_TRAP
				&& ttmp->ttyp != FAINT_TRAP
				&& ttmp->ttyp != CURSE_TRAP
				&& ttmp->ttyp != DIFFICULTY_TRAP
				&& ttmp->ttyp != SOUND_TRAP
				&& ttmp->ttyp != CASTER_TRAP
				&& ttmp->ttyp != WEAKNESS_TRAP
				&& ttmp->ttyp != ROT_THIRTEEN_TRAP
				&& ttmp->ttyp != BISHOP_TRAP
				&& ttmp->ttyp != CONFUSION_TRAP
				&& ttmp->ttyp != NUPESELL_TRAP
				&& ttmp->ttyp != DROP_TRAP
				&& ttmp->ttyp != DSTW_TRAP
				&& ttmp->ttyp != STATUS_TRAP
				&& ttmp->ttyp != PESTILENCE_TRAP
				&& ttmp->ttyp != FAMINE_TRAP
				&& ttmp->ttyp != ALIGNMENT_TRAP
				&& ttmp->ttyp != STAIRS_TRAP
				&& ttmp->ttyp != UNINFORMATION_TRAP
				&& ttmp->ttyp != TIMERUN_TRAP
				&& ttmp->ttyp != INTRINSIC_LOSS_TRAP
				&& ttmp->ttyp != BLOOD_LOSS_TRAP
				&& ttmp->ttyp != BAD_EFFECT_TRAP
				&& ttmp->ttyp != MULTIPLY_TRAP
				&& ttmp->ttyp != AUTO_VULN_TRAP
				&& ttmp->ttyp != TELE_ITEMS_TRAP
				&& ttmp->ttyp != NASTINESS_TRAP
				&& ttmp->ttyp != RECURSION_TRAP
				&& ttmp->ttyp != RESPAWN_TRAP
				&& ttmp->ttyp != WARP_ZONE
				&& ttmp->ttyp != CAPTCHA_TRAP
				&& ttmp->ttyp != MIND_WIPE_TRAP
				&& ttmp->ttyp != LOCK_TRAP
				&& ttmp->ttyp != MAGIC_CANCELLATION_TRAP
				&& ttmp->ttyp != FARLOOK_TRAP
				&& ttmp->ttyp != GATEWAY_FROM_HELL
				&& ttmp->ttyp != GROWING_TRAP
				&& ttmp->ttyp != COOLING_TRAP
				&& ttmp->ttyp != BAR_TRAP
				&& ttmp->ttyp != LOCKING_TRAP
				&& ttmp->ttyp != AIR_TRAP
				&& ttmp->ttyp != TERRAIN_TRAP
				&& ttmp->ttyp != DISCONNECT_TRAP
				&& ttmp->ttyp != INTERFACE_SCREW_TRAP
				&& ttmp->ttyp != DIMNESS_TRAP
				&& ttmp->ttyp != EVIL_ARTIFACT_TRAP
				&& ttmp->ttyp != BOSSFIGHT_TRAP
				&& ttmp->ttyp != ENTIRE_LEVEL_TRAP
				&& ttmp->ttyp != BONES_TRAP
				&& ttmp->ttyp != RODNEY_TRAP
				&& ttmp->ttyp != AUTOCURSE_TRAP
				&& ttmp->ttyp != HIGHLEVEL_TRAP
				&& ttmp->ttyp != SPELL_FORGETTING_TRAP
				&& ttmp->ttyp != SOUND_EFFECT_TRAP

				&& ttmp->ttyp != LOOTCUT_TRAP
				&& ttmp->ttyp != MONSTER_SPEED_TRAP
				&& ttmp->ttyp != SCALING_TRAP
				&& ttmp->ttyp != ENMITY_TRAP
				&& ttmp->ttyp != WHITE_SPELL_TRAP
				&& ttmp->ttyp != COMPLETE_GRAY_SPELL_TRAP
				&& ttmp->ttyp != QUASAR_TRAP
				&& ttmp->ttyp != MOMMA_TRAP
				&& ttmp->ttyp != HORROR_TRAP
				&& ttmp->ttyp != ARTIFICER_TRAP
				&& ttmp->ttyp != WEREFORM_TRAP
				&& ttmp->ttyp != NON_PRAYER_TRAP
				&& ttmp->ttyp != EVIL_PATCH_TRAP
				&& ttmp->ttyp != HARD_MODE_TRAP
				&& ttmp->ttyp != SECRET_ATTACK_TRAP
				&& ttmp->ttyp != EATER_TRAP
				&& ttmp->ttyp != COVETOUSNESS_TRAP
				&& ttmp->ttyp != NOT_SEEN_TRAP
				&& ttmp->ttyp != DARK_MODE_TRAP
				&& ttmp->ttyp != ANTISEARCH_TRAP
				&& ttmp->ttyp != HOMICIDE_TRAP
				&& ttmp->ttyp != NASTY_NATION_TRAP
				&& ttmp->ttyp != WAKEUP_CALL_TRAP
				&& ttmp->ttyp != GRAYOUT_TRAP
				&& ttmp->ttyp != GRAY_CENTER_TRAP
				&& ttmp->ttyp != CHECKERBOARD_TRAP
				&& ttmp->ttyp != CLOCKWISE_SPIN_TRAP
				&& ttmp->ttyp != COUNTERCLOCKWISE_SPIN_TRAP
				&& ttmp->ttyp != LAG_TRAP
				&& ttmp->ttyp != BLESSCURSE_TRAP
				&& ttmp->ttyp != DE_LIGHT_TRAP
				&& ttmp->ttyp != DISCHARGE_TRAP
				&& ttmp->ttyp != TRASHING_TRAP
				&& ttmp->ttyp != FILTERING_TRAP
				&& ttmp->ttyp != DEFORMATTING_TRAP
				&& ttmp->ttyp != FLICKER_STRIP_TRAP
				&& ttmp->ttyp != UNDRESSING_TRAP
				&& ttmp->ttyp != HYPERBLUEWALL_TRAP
				&& ttmp->ttyp != NOLITE_TRAP
				&& ttmp->ttyp != PARANOIA_TRAP
				&& ttmp->ttyp != FLEECESCRIPT_TRAP
				&& ttmp->ttyp != INTERRUPT_TRAP
				&& ttmp->ttyp != DUSTBIN_TRAP
				&& ttmp->ttyp != MANA_BATTERY_TRAP
				&& ttmp->ttyp != MONSTERFINGERS_TRAP
				&& ttmp->ttyp != MISCAST_TRAP
				&& ttmp->ttyp != MESSAGE_SUPPRESSION_TRAP
				&& ttmp->ttyp != STUCK_ANNOUNCEMENT_TRAP
				&& ttmp->ttyp != BLOODTHIRSTY_TRAP
				&& ttmp->ttyp != MAXIMUM_DAMAGE_TRAP
				&& ttmp->ttyp != LATENCY_TRAP
				&& ttmp->ttyp != STARLIT_TRAP
				&& ttmp->ttyp != KNOWLEDGE_TRAP
				&& ttmp->ttyp != HIGHSCORE_TRAP
				&& ttmp->ttyp != PINK_SPELL_TRAP
				&& ttmp->ttyp != GREEN_SPELL_TRAP
				&& ttmp->ttyp != EVC_TRAP
				&& ttmp->ttyp != UNDERLAYER_TRAP
				&& ttmp->ttyp != DAMAGE_METER_TRAP
				&& ttmp->ttyp != ARBITRARY_WEIGHT_TRAP
				&& ttmp->ttyp != FUCKED_INFO_TRAP
				&& ttmp->ttyp != BLACK_SPELL_TRAP
				&& ttmp->ttyp != CYAN_SPELL_TRAP
				&& ttmp->ttyp != HEAP_TRAP
				&& ttmp->ttyp != BLUE_SPELL_TRAP
				&& ttmp->ttyp != TRON_TRAP
				&& ttmp->ttyp != RED_SPELL_TRAP
				&& ttmp->ttyp != TOO_HEAVY_TRAP
				&& ttmp->ttyp != ELONGATION_TRAP
				&& ttmp->ttyp != WRAPOVER_TRAP
				&& ttmp->ttyp != DESTRUCTION_TRAP
				&& ttmp->ttyp != MELEE_PREFIX_TRAP
				&& ttmp->ttyp != AUTOMORE_TRAP
				&& ttmp->ttyp != UNFAIR_ATTACK_TRAP

				&& ttmp->ttyp != LOUDSPEAKER
				&& ttmp->ttyp != NEST_TRAP
				&& ttmp->ttyp != CYANIDE_TRAP
				&& ttmp->ttyp != LASER_TRAP
				&& ttmp->ttyp != FART_TRAP
				&& ttmp->ttyp != CONFUSE_TRAP
				&& ttmp->ttyp != STUN_TRAP
				&& ttmp->ttyp != HALLUCINATION_TRAP
				&& ttmp->ttyp != PETRIFICATION_TRAP
				&& ttmp->ttyp != NUMBNESS_TRAP
				&& ttmp->ttyp != FREEZING_TRAP
				&& ttmp->ttyp != BURNING_TRAP
				&& ttmp->ttyp != FEAR_TRAP
				&& ttmp->ttyp != BLINDNESS_TRAP
				&& ttmp->ttyp != GLIB_TRAP
				&& ttmp->ttyp != SLIME_TRAP
				&& ttmp->ttyp != INERTIA_TRAP
				&& ttmp->ttyp != TIME_TRAP
				&& ttmp->ttyp != LYCANTHROPY_TRAP
				&& ttmp->ttyp != UNLIGHT_TRAP
				&& ttmp->ttyp != ELEMENTAL_TRAP
				&& ttmp->ttyp != ESCALATING_TRAP
				&& ttmp->ttyp != NEGATIVE_TRAP
				&& ttmp->ttyp != MANA_TRAP
				&& ttmp->ttyp != SIN_TRAP
				&& ttmp->ttyp != DESTROY_ARMOR_TRAP
				&& ttmp->ttyp != DIVINE_ANGER_TRAP
				&& ttmp->ttyp != GENETIC_TRAP
				&& ttmp->ttyp != MISSINGNO_TRAP
				&& ttmp->ttyp != CANCELLATION_TRAP
				&& ttmp->ttyp != HOSTILITY_TRAP
				&& ttmp->ttyp != BOSS_TRAP
				&& ttmp->ttyp != WISHING_TRAP
				&& ttmp->ttyp != RECURRING_AMNESIA_TRAP
				&& ttmp->ttyp != BIGSCRIPT_TRAP
				&& ttmp->ttyp != BANK_TRAP
				&& ttmp->ttyp != ONLY_TRAP
				&& ttmp->ttyp != MAP_TRAP
				&& ttmp->ttyp != TECH_TRAP
				&& ttmp->ttyp != DISENCHANT_TRAP
				&& ttmp->ttyp != VERISIERT
				&& ttmp->ttyp != CHAOS_TRAP
				&& ttmp->ttyp != MUTENESS_TRAP
				&& ttmp->ttyp != NTLL_TRAP
				&& ttmp->ttyp != ENGRAVING_TRAP
				&& ttmp->ttyp != MAGIC_DEVICE_TRAP
				&& ttmp->ttyp != BOOK_TRAP
				&& ttmp->ttyp != LEVEL_TRAP
				&& ttmp->ttyp != QUIZ_TRAP

				&& ttmp->ttyp != BOMB_TRAP
				&& ttmp->ttyp != EARTHQUAKE_TRAP
				&& ttmp->ttyp != GLUE_TRAP
				&& ttmp->ttyp != GUILLOTINE_TRAP
				&& ttmp->ttyp != BISECTION_TRAP
				&& ttmp->ttyp != VOLT_TRAP
				&& ttmp->ttyp != HORDE_TRAP
				&& ttmp->ttyp != IMMOBILITY_TRAP
				&& ttmp->ttyp != GREEN_GLYPH
				&& ttmp->ttyp != BLUE_GLYPH
				&& ttmp->ttyp != YELLOW_GLYPH
				&& ttmp->ttyp != ORANGE_GLYPH
				&& ttmp->ttyp != BLACK_GLYPH
				&& ttmp->ttyp != PURPLE_GLYPH

				&& ttmp->ttyp != METABOLIC_TRAP
				&& ttmp->ttyp != TRAP_OF_NO_RETURN
				&& ttmp->ttyp != EGOTRAP
				&& ttmp->ttyp != FAST_FORWARD_TRAP
				&& ttmp->ttyp != TRAP_OF_ROTTENNESS
				&& ttmp->ttyp != UNSKILLED_TRAP
				&& ttmp->ttyp != LOW_STATS_TRAP
				&& ttmp->ttyp != TRAINING_TRAP
				&& ttmp->ttyp != EXERCISE_TRAP
				&& ttmp->ttyp != FALLING_LOADSTONE_TRAP
				&& ttmp->ttyp != SUMMON_UNDEAD_TRAP
				&& ttmp->ttyp != FALLING_NASTYSTONE_TRAP

				&& ttmp->ttyp != SPINED_BALL_TRAP
				&& ttmp->ttyp != PENDULUM_TRAP
				&& ttmp->ttyp != TURN_TABLE
				&& ttmp->ttyp != SCENT_TRAP
				&& ttmp->ttyp != BANANA_TRAP
				&& ttmp->ttyp != FALLING_TUB_TRAP
				&& ttmp->ttyp != ALARM
				&& ttmp->ttyp != CALTROPS_TRAP
				&& ttmp->ttyp != BLADE_WIRE
				&& ttmp->ttyp != MAGNET_TRAP
				&& ttmp->ttyp != SLINGSHOT_TRAP
				&& ttmp->ttyp != CANNON_TRAP
				&& ttmp->ttyp != VENOM_SPRINKLER
				&& ttmp->ttyp != FUMAROLE

				&& ttmp->ttyp != NEXUS_TRAP
				&& ttmp->ttyp != LEG_TRAP
				&& ttmp->ttyp != ARTIFACT_JACKPOT_TRAP
				&& ttmp->ttyp != MAP_AMNESIA_TRAP
				&& ttmp->ttyp != SPREADING_TRAP
				&& ttmp->ttyp != ADJACENT_TRAP
				&& ttmp->ttyp != SUPERTHING_TRAP
				&& ttmp->ttyp != LEVITATION_TRAP
				&& ttmp->ttyp != BOWEL_CRAMPS_TRAP
				&& ttmp->ttyp != UNEQUIPPING_TRAP
				&& ttmp->ttyp != GOOD_ARTIFACT_TRAP
				&& ttmp->ttyp != GENDER_TRAP
				&& ttmp->ttyp != TRAP_OF_OPPOSITE_ALIGNMENT
				&& ttmp->ttyp != SINCOUNT_TRAP
				&& ttmp->ttyp != WRENCHING_TRAP
				&& ttmp->ttyp != TRACKER_TRAP
				&& ttmp->ttyp != NURSE_TRAP
				&& ttmp->ttyp != BACK_TO_START_TRAP
				&& ttmp->ttyp != NEMESIS_TRAP
				&& ttmp->ttyp != STREW_TRAP
				&& ttmp->ttyp != OUTTA_DEPTH_TRAP
				&& ttmp->ttyp != PUNISHMENT_TRAP
				&& ttmp->ttyp != BOON_TRAP
				&& ttmp->ttyp != FOUNTAIN_TRAP
				&& ttmp->ttyp != THRONE_TRAP
				&& ttmp->ttyp != ARABELLA_SPEAKER
				&& ttmp->ttyp != FEMMY_TRAP
				&& ttmp->ttyp != MADELEINE_TRAP
				&& ttmp->ttyp != MARLENA_TRAP
				&& ttmp->ttyp != ANASTASIA_TRAP
				&& ttmp->ttyp != FILLER_TRAP
				&& ttmp->ttyp != TOXIC_VENOM_TRAP
				&& ttmp->ttyp != INSANITY_TRAP
				&& ttmp->ttyp != MADNESS_TRAP
				&& ttmp->ttyp != JESSICA_TRAP
				&& ttmp->ttyp != SOLVEJG_TRAP
				&& ttmp->ttyp != WENDY_TRAP
				&& ttmp->ttyp != KATHARINA_TRAP
				&& ttmp->ttyp != ELENA_TRAP
				&& ttmp->ttyp != THAI_TRAP
				&& ttmp->ttyp != ELIF_TRAP
				&& ttmp->ttyp != NADJA_TRAP
				&& ttmp->ttyp != SANDRA_TRAP
				&& ttmp->ttyp != NATALJE_TRAP
				&& ttmp->ttyp != JEANETTA_TRAP
				&& ttmp->ttyp != YVONNE_TRAP
				&& ttmp->ttyp != MAURAH_TRAP

				&& ttmp->ttyp != ELEMENTAL_PORTAL
				&& ttmp->ttyp != GIRLINESS_TRAP
				&& ttmp->ttyp != FUMBLING_TRAP
				&& ttmp->ttyp != EGOMONSTER_TRAP
				&& ttmp->ttyp != FLOODING_TRAP
				&& ttmp->ttyp != MONSTER_CUBE
				&& ttmp->ttyp != CURSED_GRAVE
				&& ttmp->ttyp != LIMITATION_TRAP
				&& ttmp->ttyp != WEAK_SIGHT_TRAP
				&& ttmp->ttyp != RANDOM_MESSAGE_TRAP

				&& ttmp->ttyp != DESECRATION_TRAP
				&& ttmp->ttyp != STARVATION_TRAP
				&& ttmp->ttyp != DROPLESS_TRAP
				&& ttmp->ttyp != LOW_EFFECT_TRAP
				&& ttmp->ttyp != INVISIBLE_TRAP
				&& ttmp->ttyp != GHOST_WORLD_TRAP
				&& ttmp->ttyp != DEHYDRATION_TRAP
				&& ttmp->ttyp != HATE_TRAP
				&& ttmp->ttyp != SPACEWARS_TRAP
				&& ttmp->ttyp != TEMPORARY_RECURSION_TRAP
				&& ttmp->ttyp != TOTTER_TRAP
				&& ttmp->ttyp != NONINTRINSICAL_TRAP
				&& ttmp->ttyp != DROPCURSE_TRAP
				&& ttmp->ttyp != NAKEDNESS_TRAP
				&& ttmp->ttyp != ANTILEVEL_TRAP
				&& ttmp->ttyp != VENTILATOR
				&& ttmp->ttyp != STEALER_TRAP
				&& ttmp->ttyp != REBELLION_TRAP
				&& ttmp->ttyp != CRAP_TRAP
				&& ttmp->ttyp != MISFIRE_TRAP
				&& ttmp->ttyp != TRAP_OF_WALLS

				&& ttmp->ttyp != KOP_CUBE
				&& ttmp->ttyp != BOSS_SPAWNER
				&& ttmp->ttyp != CONTAMINATION_TRAP

				&& ((ttmp->ttyp != PIT
				    && ttmp->ttyp != SPIKED_PIT
				    && ttmp->ttyp != GIANT_CHASM
				    && ttmp->ttyp != SHIT_PIT
				    && ttmp->ttyp != MANA_PIT
				    && ttmp->ttyp != ANOXIC_PIT
				    && ttmp->ttyp != SHAFT_TRAP
				    && ttmp->ttyp != TRAPDOOR
				    && ttmp->ttyp != HOLE)
				      || (!is_flyer(mdat) && (!mon->egotype_flying) 
				    && !is_floater(mdat)
				    && !is_clinger(mdat))
				      || In_sokoban(&u.uz))

				&& (ttmp->ttyp != DEATH_TRAP ||
				    (nonliving(mon->data) || is_demon(mon->data) || resists_death(mon) || mon->data->msound == MS_NEMESIS || resists_magm(mon)) )

				&& (ttmp->ttyp != DISINTEGRATION_TRAP ||
				    (!resists_disint(mon)) )

				&& (ttmp->ttyp != DRAIN_TRAP ||
				    (!resists_drli(mon)) )

				&& (ttmp->ttyp != SLP_GAS_TRAP ||
				    (!resists_sleep(mon) && !breathless(mdat) && (!mon->egotype_undead) ) )
				&& (ttmp->ttyp != POISON_GAS_TRAP ||
				    (!resists_poison(mon) && !breathless(mdat) && (!mon->egotype_undead) ) )
				&& (ttmp->ttyp != SLOW_GAS_TRAP ||
				    (!breathless(mdat) && (!mon->egotype_undead) ) )
				&& (ttmp->ttyp != BEAR_TRAP ||
				    (mdat->msize > MZ_SMALL &&
				     !amorphous(mdat) && !is_flyer(mdat) && (!mon->egotype_flying) ))
				&& (ttmp->ttyp != FIRE_TRAP ||
				    !resists_fire(mon))
				&& (ttmp->ttyp != SHOCK_TRAP ||
				    !resists_elec(mon))
				&& (ttmp->ttyp != ICE_TRAP ||
				    !resists_cold(mon))
				&& (ttmp->ttyp != SQKY_BOARD || (!is_flyer(mdat) && (!mon->egotype_flying) ))
				&& (ttmp->ttyp != ACID_POOL || (!is_flyer(mdat) && (!mon->egotype_flying) && !is_floater(mdat) && !resists_acid(mon)) )
				&& (ttmp->ttyp != WATER_POOL || (!is_flyer(mdat) && (!mon->egotype_flying) && !is_floater(mdat) && !is_swimmer(mdat) && !mon->egotype_watersplasher && !amphibious(mdat) && !breathless(mdat) && (!mon->egotype_undead) ) )
				&& (ttmp->ttyp != WEB || (!amorphous(mdat) && !mon->egotype_webber &&
				    !webmaker(mdat) && !dmgtype(mdat, AD_WEBS) ))
			) {
			    if (!(flag & ALLOW_TRAPS)) {
				/* No more mtrapseen weirdness - just have a random chance of avoiding the trap --Amy */
				if (/*mon->mtrapseen & (1L << (ttmp->ttyp - 1))*/!rn2(3))
				    continue;
			    }
			    info[cnt] |= ALLOW_TRAPS;
			}
		    }
		}
		poss[cnt].x = nx;
		poss[cnt].y = ny;
		cnt++;
	    }
	}

	/* The following code assumes that all "wantlava" monsters are also "wantpool". --Amy */
	if(!cnt && wantlava && !is_lava(x,y) && !is_styxriver(x,y) && !is_waterypool(x,y) && !is_watertunnel(x,y) ) {
		wantlava = FALSE;
		wantpool = FALSE;
		goto nexttry;
	}
	/* The reverse is obviously not true; this check had to be changed to make sure lava-loving monsters don't leave their pools of lava... */
	if(!cnt && wantpool && !wantlava && !is_waterypool(x,y) && !is_watertunnel(x,y)) {
		wantpool = FALSE;
		goto nexttry;
	}
	return(cnt);
}

#endif /* OVL0 */
#ifdef OVL1

/* Monster against monster special attacks; for the specified monster
   combinations, this allows one monster to attack another adjacent one
   in the absence of Conflict.  There is no provision for targetting
   other monsters; just hand to hand fighting when they happen to be
   next to each other. */
STATIC_OVL long
mm_aggression(magr, mdef)
struct monst *magr,	/* monster that is currently deciding where to move */
	     *mdef;	/* another monster which is next to it */
{

	if (Race_if(PM_ALBAE)) return 0L; /* if you're an albae, everything hates you more than anything else --Amy */
	if (Role_if(PM_CRUEL_ABUSER) && Qstatf(killed_nemesis) ) return 0L; /* or if you killed the abuser nemesis */
	if (Aggravate_monster && rn2(5) ) return 0L; /* aggravate monster makes monsters much more likely to target you and ignore everything else */

	/* supposedly purple worms are attracted to shrieking because they
	   like to eat shriekers, so attack the latter when feasible */
	if (magr->data == &mons[PM_PURPLE_WORM] &&
		mdef->data == &mons[PM_SHRIEKER])
	    return ALLOW_M|ALLOW_TM;

	/* elves vs. orcs */
	if(magr->data->mflags2 & M2_ELF && mdef->data->mflags2 & M2_ORC)
		return ALLOW_M|ALLOW_TM;
	/* and vice versa */
	if(mdef->data->mflags2 & M2_ELF && magr->data->mflags2 & M2_ORC)
		return ALLOW_M|ALLOW_TM;

	/* angels vs. demons */
	if(magr->data->mlet==S_ANGEL && mdef->data->mflags2 & M2_DEMON)
		return ALLOW_M|ALLOW_TM;
	/* and vice versa */
	if(mdef->data->mlet==S_ANGEL && magr->data->mflags2 & M2_DEMON)
		return ALLOW_M|ALLOW_TM;

	/* woodchucks vs. The Oracle */
	if(magr->data == &mons[PM_WOODCHUCK] && mdef->data == &mons[PM_ORACLE])
		return ALLOW_M|ALLOW_TM;

	/* aurors vs. death eaters */
	if(magr->data == &mons[PM_AUROR] && mdef->data == &mons[PM_FELLOW_DEATH_EATER])
		return ALLOW_M|ALLOW_TM;
	if(mdef->data == &mons[PM_AUROR] && magr->data == &mons[PM_FELLOW_DEATH_EATER])
		return ALLOW_M|ALLOW_TM;

	/* Stormtroopers vs. Padawans */
	if(magr->data == &mons[PM_STORMTROOPER] && mdef->data == &mons[PM_PADAWAN])
		return ALLOW_M|ALLOW_TM;
	/* and vice versa */
	if(mdef->data == &mons[PM_STORMTROOPER] && magr->data == &mons[PM_PADAWAN])
		return ALLOW_M|ALLOW_TM;
	/* Stormtroopers vs. Jedi */
	if(magr->data == &mons[PM_STORMTROOPER] && mdef->data == &mons[PM_JEDI])
		return ALLOW_M|ALLOW_TM;
	/* and vice versa */
	if(mdef->data == &mons[PM_STORMTROOPER] && magr->data == &mons[PM_JEDI])
		return ALLOW_M|ALLOW_TM;
	/* Stormtroopers vs. The Jedi Master */
	//if(magr->data == &mons[PM_STORMTROOPER] && mdef->data == &mons[PM_THE_JEDI_MASTER])
		//return ALLOW_M|ALLOW_TM;
	/* and vice versa */
	//if(mdef->data == &mons[PM_STORMTROOPER] && magr->data == &mons[PM_THE_JEDI_MASTER])
		//return ALLOW_M|ALLOW_TM;
	/* Jedi vs. Lord Sidious */
	if(magr->data == &mons[PM_LORD_SIDIOUS] && mdef->data == &mons[PM_JEDI])
		return ALLOW_M|ALLOW_TM;
	/* and vice versa */
	if(mdef->data == &mons[PM_LORD_SIDIOUS] && magr->data == &mons[PM_JEDI])
		return ALLOW_M|ALLOW_TM;

	return 0L;
}

boolean
monnear(mon, x, y)
register struct monst *mon;
register int x,y;
/* Is the square close enough for the monster to move or attack into? */
{
	register int distance = dist2(mon->mx, mon->my, x, y);
	if (distance==2 && ( mon->data==&mons[PM_GRID_BUG] || mon->data==&mons[PM_WEREGRIDBUG] || mon->data==&mons[PM_GRID_XORN] || mon->data==&mons[PM_STONE_BUG] || mon->data==&mons[PM_NATURAL_BUG] || mon->data==&mons[PM_MELEE_BUG] || mon->data==&mons[PM_VORPAL_GRID_BUG] || mon->data==&mons[PM_WEAPON_BUG] || mon->data==&mons[PM_SPECIFIC_BUG] || mon->data==&mons[PM_CONTAMINATED_BUG] || (uarmf && !rn2(10) && OBJ_DESCR(objects[uarmf->otyp]) && ( !strcmp(OBJ_DESCR(objects[uarmf->otyp]), "chess boots") || !strcmp(OBJ_DESCR(objects[uarmf->otyp]), "shakhmatnyye sapogi") || !strcmp(OBJ_DESCR(objects[uarmf->otyp]), "shaxmat chizilmasin") ) ) ) ) return 0;
	return((boolean)(distance < 3));
}

/* really free dead monsters */
void
dmonsfree()
{
    struct monst **mtmp;
    int count = 0;

    for (mtmp = &fmon; *mtmp;) {
	if ((*mtmp)->mhp <= 0) {
	    struct monst *freetmp = *mtmp;
	    *mtmp = (*mtmp)->nmon;
	    dealloc_monst(freetmp);
	    count++;
	} else
	    mtmp = &(*mtmp)->nmon;
    }

    if (count != iflags.purge_monsters)
	impossible("dmonsfree: %d removed doesn't match %d pending",
		   count, iflags.purge_monsters);
    iflags.purge_monsters = 0;
}

#endif /* OVL1 */
#ifdef OVLB

/* called when monster is moved to larger structure */
void
replmon(mtmp, mtmp2)
register struct monst *mtmp, *mtmp2;
{
    struct obj *otmp;
    long unpolytime; /* WAC */

    /* transfer the monster's inventory */
    for (otmp = mtmp2->minvent; otmp; otmp = otmp->nobj) {
#ifdef DEBUG
	if (otmp->where != OBJ_MINVENT || otmp->ocarry != mtmp)
	    panic("replmon: minvent inconsistency");
#endif
	otmp->ocarry = mtmp2;
    }
    mtmp->minvent = 0;

    /* remove the old monster from the map and from `fmon' list */
    relmon(mtmp);

    /* finish adding its replacement */
    if (mtmp == u.usteed) ; else	/* don't place steed onto the map */
    place_monster(mtmp2, mtmp2->mx, mtmp2->my);
    if (mtmp2->wormno)	    /* update level.monsters[wseg->wx][wseg->wy] */
	place_wsegs(mtmp2); /* locations to mtmp2 not mtmp. */
    if (emits_light(mtmp2->data)) {
	/* since this is so rare, we don't have any `mon_move_light_source' */
	new_light_source(mtmp2->mx, mtmp2->my,
			 emits_light(mtmp2->data),
			 LS_MONSTER, (void *)mtmp2);
	/* here we rely on the fact that `mtmp' hasn't actually been deleted */
	del_light_source(LS_MONSTER, (void *)mtmp);
    }
    /* If poly'ed,  move polytimer along */
    if (unpolytime = (stop_timer(UNPOLY_MON, (void *) mtmp))) {
        (void) start_timer(unpolytime, TIMER_MONSTER, UNPOLY_MON,
                        (void *) mtmp2);
    }
    mtmp2->nmon = fmon;
    fmon = mtmp2;
    if (u.ustuck == mtmp) setustuck(mtmp2);
    if (u.usteed == mtmp) u.usteed = mtmp2;
    if (mtmp2->isshk) replshk(mtmp,mtmp2);

    /* discard the old monster */
    dealloc_monst(mtmp);
}

/* release mon from display and monster list */
void
relmon(mon)
register struct monst *mon;
{
	register struct monst *mtmp;

	if (fmon == (struct monst *)0)  panic ("relmon: no fmon available.");

	remove_monster(mon->mx, mon->my);

	if(mon == fmon) fmon = fmon->nmon;
	else {
		for(mtmp = fmon; mtmp && mtmp->nmon != mon; mtmp = mtmp->nmon) ;
		if(mtmp)    mtmp->nmon = mon->nmon;
		else	    panic("relmon: mon not in list.");
	}
}

/* remove effects of mtmp from other data structures */
STATIC_OVL void
m_detach(mtmp, mptr)
struct monst *mtmp;
struct permonst *mptr;	/* reflects mtmp->data _prior_ to mtmp's death */
{
 	mon_stop_timers(mtmp);
	if (mtmp->mleashed) m_unleash(mtmp, FALSE);
	    /* to prevent an infinite relobj-flooreffects-hmon-killed loop */
	mtmp->mtrapped = 0;
	mtmp->mhp = 0; /* simplify some tests: force mhp to 0 */
	relobj(mtmp, 0, FALSE);
	remove_monster(mtmp->mx, mtmp->my);
	if (emits_light(mptr))
	    del_light_source(LS_MONSTER, (void *)mtmp);
	newsym(mtmp->mx,mtmp->my);
	unstuck(mtmp);
	fill_pit(mtmp->mx, mtmp->my);

	if(mtmp->isshk) shkgone(mtmp);
	if(mtmp->wormno) wormgone(mtmp);
	iflags.purge_monsters++;
}

/* find the worn amulet of life saving which will save a monster */
struct obj *
mlifesaver(mon)
struct monst *mon;
{
	/*if (!nonliving(mon->data)) {*/
	    struct obj *otmp = which_armor(mon, W_AMUL);

	    if (otmp && otmp->otyp == AMULET_OF_LIFE_SAVING)
		return otmp;
	/*}*/
	return (struct obj *)0;
}

STATIC_OVL void
lifesaved_monster(mtmp)
struct monst *mtmp;
{
	int visible;
	struct obj *lifesave = mlifesaver(mtmp);

	if (lifesave) {
		/* not canseemon; amulets are on the head, so you don't want */
		/* to show this for a long worm with only a tail visible. */
		/* Nor do you check invisibility, because glowing and disinte- */
		/* grating amulets are always visible. */
		/* [ALI] Always treat swallower as visible for consistency */
		/* with unpoly_monster(). */
		visible = u.uswallow && u.ustuck == mtmp ||
			cansee(mtmp->mx, mtmp->my);
		if (visible) {
			pline("But wait...");
			pline("%s medallion begins to glow!",
				s_suffix(Monnam(mtmp)));
			makeknown(AMULET_OF_LIFE_SAVING);
			if (attacktype(mtmp->data, AT_EXPL)
			    || attacktype(mtmp->data, AT_BOOM))
				pline("%s reconstitutes!", Monnam(mtmp));
			else
				pline("%s looks much better!", Monnam(mtmp));
			pline_The("medallion crumbles to dust!");
		}
		m_useup(mtmp, lifesave);
		mtmp->mcanmove = 1;
		mtmp->mfrozen = 0;
		if (mtmp->mtame && !mtmp->isminion) {
			wary_dog(mtmp, FALSE);
		}
		if (mtmp->mhpmax <= 0) mtmp->mhpmax = 10;
		mtmp->mhp = mtmp->mhpmax;
		if (mvitals[monsndx(mtmp->data)].mvflags & G_GENOD) {
			if (visible)
			    pline("Unfortunately %s is still genocided...",
				mon_nam(mtmp));
		} else
			return;
	} else if (mtmp->egotype_lifesaver && rn2(5) ) {
		visible = u.uswallow && u.ustuck == mtmp ||
			cansee(mtmp->mx, mtmp->my);
		if (visible) {
			pline("But wait...");
			pline("%s lifesaves!", Monnam(mtmp));
			if (attacktype(mtmp->data, AT_EXPL)
			    || attacktype(mtmp->data, AT_BOOM))
				pline("%s reconstitutes!", Monnam(mtmp));
			else
				pline("%s looks much better!", Monnam(mtmp));
		}
		mtmp->mcanmove = 1;
		mtmp->mfrozen = 0;
		if (mtmp->mtame && !mtmp->isminion) {
			wary_dog(mtmp, FALSE);
		}
		if (mtmp->mhpmax <= 0) mtmp->mhpmax = 10;
		mtmp->mhp = mtmp->mhpmax;
		if (mvitals[monsndx(mtmp->data)].mvflags & G_GENOD) {
			if (visible)
			    pline("Unfortunately %s is still genocided...",
				mon_nam(mtmp));
		} else
			return;

	}
	mtmp->mhp = 0;
}

/* WAC -- undo polymorph */
static void
unpoly_monster(mtmp)
struct monst *mtmp;
{
	int visible;
	char buf[BUFSZ];

	sprintf(buf, Monnam(mtmp));

	/* If there is a timer == monster was poly'ed */
	if (stop_timer(UNPOLY_MON, (void *) mtmp)) {
	    /* [ALI] Always treat swallower as visible so that the message
	     * indicating that the monster hasn't died comes _before_ any
	     * message about breaking out of the "new" monster.
	     */
	    visible = u.uswallow && u.ustuck == mtmp || cansee(mtmp->mx,mtmp->my);
	    mtmp->mhp = mtmp->mhpmax;
	    if (visible)
		pline("But wait...");
	    if ((newcham(mtmp, &mons[mtmp->oldmonnm], FALSE, visible)) && rn2(25))
		mtmp->mhp = mtmp->mhpmax/2; /* 4% risk of the unpolymorph killing the monster. --Amy */
	    else {
		if (visible)
		    pline("%s shudders!", Monnam(mtmp));
		mtmp->mhp = 0;
	    }
	}
}

void
mondead(mtmp)
register struct monst *mtmp;
{
	struct permonst *mptr;
	int tmp;
	int copcnt; /* Angry shopkeepers and guards will send in the police and sephirah! --Amy */
	int tryct = 0;
	int x, y;
	int rtrap;

	/* WAC just in case caller forgot to...*/
	if (mtmp->mhp) mtmp->mhp = -1;

	if (mtmp->isshk && !strncmpi(shkname(mtmp), "Izchak", 6) ) { /* Thou must not kill Izchak or allow him to die. --Amy */

		adjalign(-500);
		change_luck(-10);
		You_feel("the air around you grow charged...");
		pline("Suddenly, you realize that %s has noticed you...", u_gname());
		/* Throw everything we have at the player */
		u.ugangr++; u.ugangr++; u.ugangr++; u.ugangr++; u.ugangr++; u.ugangr++; u.ugangr++; u.ugangr++; u.ugangr++;
		prayer_done();
		god_zaps_you(u.ualign.type);
	}

	if(mtmp->isgd) {
		/* if we're going to abort the death, it *must* be before
		 * the m_detach or there will be relmon problems later */
		if(!grddead(mtmp)) return;
	}

	mptr = mtmp->data;

	/* WAC First check that monster can unpoly */
	unpoly_monster(mtmp);
	if (mtmp->mhp > 0) return;

	lifesaved_monster(mtmp);
	if (mtmp->mhp > 0) return;

	/* Player is thrown from his steed when it dies */
	if (mtmp == u.usteed)
		dismount_steed(DISMOUNT_GENERIC);

	mptr = mtmp->data;		/* save this for m_detach() */
	/* restore chameleon, lycanthropes to true form at death */
	if (mtmp->cham)
	    set_mon_data(mtmp, &mons[cham_to_pm[mtmp->cham]], -1);
	else if (mtmp->data == &mons[PM_WEREJACKAL])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREJACKAL], -1);
	else if (mtmp->data == &mons[PM_WEREWOLF])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREWOLF], -1);
	else if (mtmp->data == &mons[PM_WERERAT])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERERAT], -1);
	else if (mtmp->data == &mons[PM_WERERABBIT])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERERABBIT], -1);
	else if (mtmp->data == &mons[PM_WEREBOAR])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREBOAR], -1);
	else if (mtmp->data == &mons[PM_WERELOCUST])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERELOCUST], -1);
	else if (mtmp->data == &mons[PM_WEREPANTHER])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREPANTHER], -1);
	else if (mtmp->data == &mons[PM_WERETIGER])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERETIGER], -1);
	else if (mtmp->data == &mons[PM_WERESNAKE])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERESNAKE], -1);
	else if (mtmp->data == &mons[PM_WERESPIDER])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERESPIDER], -1);
	else if (mtmp->data == &mons[PM_WEREGIANT])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREGIANT], -1);
	else if (mtmp->data == &mons[PM_WERELICHEN])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERELICHEN], -1);
	else if (mtmp->data == &mons[PM_WEREVORTEX])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREVORTEX], -1);
	else if (mtmp->data == &mons[PM_WERECOW])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERECOW], -1);
	else if (mtmp->data == &mons[PM_WEREBEAR])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREBEAR], -1);
	else if (mtmp->data == &mons[PM_WEREPIERCER])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREPIERCER], -1);
	else if (mtmp->data == &mons[PM_WEREPENETRATOR])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREPENETRATOR], -1);
	else if (mtmp->data == &mons[PM_WERESMASHER])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERESMASHER], -1);
	else if (mtmp->data == &mons[PM_WERENYMPH])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERENYMPH], -1);
	else if (mtmp->data == &mons[PM_WEREBEE])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREBEE], -1);
	else if (mtmp->data == &mons[PM_WEREMIMIC])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREMIMIC], -1);
	else if (mtmp->data == &mons[PM_WEREPERMAMIMIC])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREPERMAMIMIC], -1);
	else if (mtmp->data == &mons[PM_WERECOCKATRICE])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERECOCKATRICE], -1);
	else if (mtmp->data == &mons[PM_WERESOLDIERANT])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERESOLDIERANT], -1);
	else if (mtmp->data == &mons[PM_WERETROLL])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERETROLL], -1);
	else if (mtmp->data == &mons[PM_WEREBAT])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREBAT], -1);
	else if (mtmp->data == &mons[PM_WEREMINDFLAYER])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREMINDFLAYER], -1);
	else if (mtmp->data == &mons[PM_WEREGHOST])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREGHOST], -1);
	else if (mtmp->data == &mons[PM_WEREPIRANHA])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREPIRANHA], -1);
	else if (mtmp->data == &mons[PM_WEREEEL])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREEEL], -1);
	else if (mtmp->data == &mons[PM_WEREFLYFISH])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREFLYFISH], -1);
	else if (mtmp->data == &mons[PM_WEREKRAKEN])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREKRAKEN], -1);
	else if (mtmp->data == &mons[PM_WERELICH])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERELICH], -1);
	else if (mtmp->data == &mons[PM_WEREJABBERWOCK])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREJABBERWOCK], -1);
	else if (mtmp->data == &mons[PM_WEREGRIDBUG])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREGRIDBUG], -1);
	else if (mtmp->data == &mons[PM_WEREACIDBLOB])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREACIDBLOB], -1);
	else if (mtmp->data == &mons[PM_WEREFOX])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREFOX], -1);
	else if (mtmp->data == &mons[PM_WEREMONKEY])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREMONKEY], -1);
	else if (mtmp->data == &mons[PM_WEREFLOATINGEYE])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREFLOATINGEYE], -1);
	else if (mtmp->data == &mons[PM_WEREGIANTANT])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREGIANTANT], -1);
	else if (mtmp->data == &mons[PM_WEREKOBOLD])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREKOBOLD], -1);
	else if (mtmp->data == &mons[PM_WERELEPRECHAUN])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERELEPRECHAUN], -1);
	else if (mtmp->data == &mons[PM_WEREHORSE])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREHORSE], -1);
	else if (mtmp->data == &mons[PM_WERERUSTMONSTER])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERERUSTMONSTER], -1);
	else if (mtmp->data == &mons[PM_WEREBLACKLIGHT])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREBLACKLIGHT], -1);
	else if (mtmp->data == &mons[PM_WEREAUTON])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREAUTON], -1);
	else if (mtmp->data == &mons[PM_WEREGREMLIN])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREGREMLIN], -1);
	else if (mtmp->data == &mons[PM_WEREGREENSLIME])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREGREENSLIME], -1);
	else if (mtmp->data == &mons[PM_WEREJELLY])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREJELLY], -1);
	else if (mtmp->data == &mons[PM_WEREXAN])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREXAN], -1);
	else if (mtmp->data == &mons[PM_WEREMUMAK])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREMUMAK], -1);
	else if (mtmp->data == &mons[PM_WERECENTAUR])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERECENTAUR], -1);
	else if (mtmp->data == &mons[PM_WERECOUATL])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERECOUATL], -1);
	else if (mtmp->data == &mons[PM_WEREGELATINOUSCUBE])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREGELATINOUSCUBE], -1);
	else if (mtmp->data == &mons[PM_WEREAIRELEMENTAL])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREAIRELEMENTAL], -1);
	else if (mtmp->data == &mons[PM_WEREEARTHELEMENTAL])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREEARTHELEMENTAL], -1);
	else if (mtmp->data == &mons[PM_WEREFIREELEMENTAL])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREFIREELEMENTAL], -1);
	else if (mtmp->data == &mons[PM_WEREWATERELEMENTAL])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREWATERELEMENTAL], -1);
	else if (mtmp->data == &mons[PM_WEREASTRALELEMENTAL])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREASTRALELEMENTAL], -1);
	else if (mtmp->data == &mons[PM_WEREXORN])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREXORN], -1);
	else if (mtmp->data == &mons[PM_WERESTALKER])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERESTALKER], -1);
	else if (mtmp->data == &mons[PM_WEREDRACONIAN])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREDRACONIAN], -1);
	else if (mtmp->data == &mons[PM_WEREUMBERHULK])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREUMBERHULK], -1);
	else if (mtmp->data == &mons[PM_WEREVAMPIRE])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREVAMPIRE], -1);
	else if (mtmp->data == &mons[PM_WEREKILLERBEE])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREKILLERBEE], -1);
	else if (mtmp->data == &mons[PM_WEREKANGAROO])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREKANGAROO], -1);
	else if (mtmp->data == &mons[PM_WEREGRUE])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREGRUE], -1);
	else if (mtmp->data == &mons[PM_WERECOINS])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERECOINS], -1);
	else if (mtmp->data == &mons[PM_WERETRAPPER])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERETRAPPER], -1);
	else if (mtmp->data == &mons[PM_WERESHADE])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERESHADE], -1);
	else if (mtmp->data == &mons[PM_WEREDISENCHANTER])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREDISENCHANTER], -1);
	else if (mtmp->data == &mons[PM_WERENAGA])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERENAGA], -1);
	else if (mtmp->data == &mons[PM_WEREWORM])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREWORM], -1);
	else if (mtmp->data == &mons[PM_WEREDRAGON])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREDRAGON], -1);
	else if (mtmp->data == &mons[PM_WEREWEDGESANDAL])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREWEDGESANDAL], -1);
	else if (mtmp->data == &mons[PM_WEREHUGGINGBOOT])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREHUGGINGBOOT], -1);
	else if (mtmp->data == &mons[PM_WEREPEEPTOE])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREPEEPTOE], -1);
	else if (mtmp->data == &mons[PM_WERESEXYLEATHERPUMP])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERESEXYLEATHERPUMP], -1);
	else if (mtmp->data == &mons[PM_WEREBLOCKHEELEDCOMBATBOOT])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREBLOCKHEELEDCOMBATBOOT], -1);
	else if (mtmp->data == &mons[PM_WERECOMBATSTILETTO])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERECOMBATSTILETTO], -1);
	else if (mtmp->data == &mons[PM_WEREBEAUTIFULFUNNELHEELEDPUMP])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREBEAUTIFULFUNNELHEELEDPUMP], -1);
	else if (mtmp->data == &mons[PM_WEREPROSTITUTESHOE])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREPROSTITUTESHOE], -1);
	else if (mtmp->data == &mons[PM_WERESTILETTOSANDAL])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WERESTILETTOSANDAL], -1);
	else if (mtmp->data == &mons[PM_WEREUNFAIRSTILETTO])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREUNFAIRSTILETTO], -1);
	else if (mtmp->data == &mons[PM_WEREWINTERSTILETTO])
	    set_mon_data(mtmp, &mons[PM_HUMAN_WEREWINTERSTILETTO], -1);

	/* if MAXMONNO monsters of a given type have died, and it
	 * can be done, extinguish that monster.
	 *
	 * mvitals[].died does double duty as total number of dead monsters
	 * and as experience factor for the player killing more monsters.
	 * this means that a dragon dying by other means reduces the
	 * experience the player gets for killing a dragon directly; this
	 * is probably not too bad, since the player likely finagled the
	 * first dead dragon via ring of conflict or pets, and extinguishing
	 * based on only player kills probably opens more avenues of abuse
	 * for rings of conflict and such.
	 */
	/* KMH -- Yes, keep spell monsters in the count */
	tmp = monsndx(mtmp->data);

	if (tmp == PM_BLACKY) {
		if (wizard) pline("Blacky died!");
		BlackNgWalls = 0L;
	}

	if (tmp == PM_MOLDOUX__THE_DEFENCELESS_MOLD) {
		pline("This monster was under the protection of a Great Wyrm of Power!");
		verbalize("Harharhar mortal, now you DIE!!!");
	      (void) makemon(&mons[PM_GREAT_WYRM_OF_POWER], u.ux, u.uy, NO_MM_FLAGS);
	}

	if (tmp == PM_ARIANE_S_FLEECY_COMBAT_BOOT) {
		pline("This monster was under the protection of a Great Wyrm of Power!");
		verbalize("Harharhar mortal, now you DIE!!!");
	      (void) makemon(&mons[PM_GREAT_WYRM_OF_POWER], u.ux, u.uy, NO_MM_FLAGS);
	}

	if (tmp == PM_GUNNHILD_S_GENERAL_STORE) {	/* create traps on the level, disregarding special probability checks */

		while (rn2(25)) {

			for (tryct = 0; tryct < 2000; tryct++) {
				x = rn1(COLNO-3,2);
				y = rn2(ROWNO);

				if (x && y && isok(x, y) && (levl[x][y].typ > DBWALL) && !(t_at(x, y)) ) {

					rtrap = rnd(TRAPNUM-1);
					if (rtrap == MAGIC_PORTAL) rtrap = ROCKTRAP;
					if (rtrap == WISHING_TRAP) rtrap = BLINDNESS_TRAP;
					if (rtrap == ARTIFACT_JACKPOT_TRAP) rtrap = MAGIC_TRAP;
					if (rtrap == GOOD_ARTIFACT_TRAP) rtrap = WEB;
					if (rtrap == BOON_TRAP) rtrap = MAGIC_BEAM_TRAP;
					if (rtrap == LEVEL_TELEP && (level.flags.noteleport || Is_knox(&u.uz) || Is_blackmarket(&u.uz) || Is_aligned_quest(&u.uz) || In_endgame(&u.uz) || In_sokoban(&u.uz) ) ) rtrap = ANTI_MAGIC;
					if (rtrap == LEVEL_BEAMER && (level.flags.noteleport || Is_knox(&u.uz) || Is_blackmarket(&u.uz) || Is_aligned_quest(&u.uz) || In_endgame(&u.uz) || In_sokoban(&u.uz) ) ) rtrap = ANTI_MAGIC;
					if (rtrap == NEXUS_TRAP && (level.flags.noteleport || Is_knox(&u.uz) || Is_blackmarket(&u.uz) || Is_aligned_quest(&u.uz) || In_endgame(&u.uz) || In_sokoban(&u.uz) ) ) rtrap = ANTI_MAGIC;
					if (rtrap == TELEP_TRAP && level.flags.noteleport) rtrap = SQKY_BOARD;
					if (rtrap == BEAMER_TRAP && level.flags.noteleport) rtrap = SQKY_BOARD;
					if ((rtrap == TRAPDOOR || rtrap == HOLE || rtrap == SHAFT_TRAP || rtrap == CURRENT_SHAFT) && !Can_fall_thru(&u.uz) && !Is_stronghold(&u.uz) ) rtrap = ROCKTRAP;
					if (rtrap == ACTIVE_SUPERSCROLLER_TRAP) rtrap = SUPERSCROLLER_TRAP;
					if (rtrap == AUTOMATIC_SWITCHER) rtrap = UNKNOWN_TRAP;

					(void) maketrap(x, y, rtrap, 100);
					break;
					}

			}

		}

	}

	/* item masters and similar stuff shouldn't appear on the list so the player can't peek with ctrl-K! --Amy */
	if (mvitals[tmp].died < 255 && tmp != PM_ITEM_MASTER && tmp != PM_GOOD_ITEM_MASTER && tmp != PM_BAD_ITEM_MASTER && tmp != PM_SCROLLER_MASTER && tmp != PM_HOLE_MASTER && tmp != PM_BOULDER_MASTER && tmp != PM_TRAP_MASTER) mvitals[tmp].died++;

	/* if it's a (possibly polymorphed) quest leader, mark him as dead */
	if (mtmp->m_id == quest_status.leader_m_id)
	    quest_status.leader_is_dead = TRUE;
#ifdef MAIL
	/* if the mail daemon dies, no more mail delivery.  -3. */
	if (tmp == PM_MAIL_DAEMON) mvitals[tmp].mvflags |= G_GENOD;
#endif

	if (mtmp->data->mlet == S_KOP) {

	    /* Dead Kops may come back. */
	    switch(rnd( (uarmh && OBJ_DESCR(objects[uarmh->otyp]) && ( !strcmp(OBJ_DESCR(objects[uarmh->otyp]), "anti-government helmet") || !strcmp(OBJ_DESCR(objects[uarmh->otyp]), "antipravitel'stvennaya shlem") || !strcmp(OBJ_DESCR(objects[uarmh->otyp]), "aksil-hukumat dubulg'a") ) ) ? 20 : RngeAntiGovernment ? 20 : 10)) {
		case 1:	     /* returns near the stairs */
			(void) makemon(mtmp->data,xdnstair,ydnstair,MM_ADJACENTOK);
			break;
		case 2:	     /* randomly */
			(void) makemon(mtmp->data,0,0,NO_MM_FLAGS);
			break;
		case 3:	     /* returns near the stairs */
			(void) makemon(mkclass(S_KOP,0),xdnstair,ydnstair,MM_ADJACENTOK);
			break;
		case 4:	     /* randomly */
			(void) makemon(mkclass(S_KOP,0),0,0,NO_MM_FLAGS);
			break;
		default:
			break;
	    }
	}

	if ( (RespawnProblem || u.uprops[RESPAWN_BUG].extrinsic || (uarmc && uarmc->oartifact == ART_PERCENTIOEOEPSPERCENTD_THI) || have_respawnstone() ) && tmp != PM_UNFORTUNATE_VICTIM && tmp != PM_SCROLLER_MASTER && tmp != PM_BOULDER_MASTER && tmp != PM_ITEM_MASTER && tmp != PM_GOOD_ITEM_MASTER && tmp != PM_BAD_ITEM_MASTER && tmp != PM_HOLE_MASTER && tmp != PM_TRAP_MASTER && !(mtmp->data->geno & G_UNIQ) ) {
	    switch(rnd(10)) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			(void) makemon(mtmp->data,0,0,NO_MM_FLAGS);
			break;
		case 9:
			(void) makemon(mtmp->data,0,0,NO_MM_FLAGS);
			(void) makemon(mtmp->data,0,0,NO_MM_FLAGS);
			break;
		case 10:
		default:
			break;
		}
	}
	if(mtmp->iswiz) wizdead();
	if(mtmp->data->msound == MS_NEMESIS && mtmp->data->mlet != S_NEMESE && tmp != PM_TRUE_MISSINGNO && tmp != PM_ETHEREAL_MISSINGNO && tmp != PM_STARLIT_SKY  && tmp != PM_MISNAMED_STARLIT_SKY && tmp != PM_WRONG_NAMED_STARLIT_SKY && tmp != PM_ERRONEOUS_STARLIT_SKY && tmp != PM_DARK_STARLIT_SKY && tmp != PM_BLACK_STARLIT_SKY && tmp != PM_RED_STARLIT_SKY && tmp != PM_BROWN_STARLIT_SKY && tmp != PM_GREEN_STARLIT_SKY && tmp != PM_PURPLE_STARLIT_SKY && tmp != PM_YELLOW_STARLIT_SKY && tmp != PM_ORANGE_STARLIT_SKY && tmp != PM_CYAN_STARLIT_SKY && tmp != PM_VIOLET_STARLIT_SKY && tmp != PM_POLYINITOR && tmp != PM_DESTABILIZER) nemdead();

	if(tmp == PM_ANASTASIA_STEELE) { /* very bad! */

		change_luck(-10);
		u.ualign.sins += 20; 
		u.alignlim -= 20;
		adjalign(-200);
		u.ugangr++; u.ugangr++; u.ugangr++; u.ugangr++; u.ugangr++;
		prayer_done();

	}

	if(tmp == PM_SHOPKEEPER || tmp == PM_BLACK_MARKETEER || tmp == PM_GUARD) /* punishment */ {

		pline("The twit quickly called the kops, and it seems they're out to get you!");
		copcnt = rnd(monster_difficulty() ) + 1;
		if (Role_if(PM_CAMPERSTRIKER)) copcnt *= (rn2(5) ? 2 : rn2(5) ? 3 : 5);

		if (uarmh && OBJ_DESCR(objects[uarmh->otyp]) && ( !strcmp(OBJ_DESCR(objects[uarmh->otyp]), "anti-government helmet") || !strcmp(OBJ_DESCR(objects[uarmh->otyp]), "antipravitel'stvennaya shlem") || !strcmp(OBJ_DESCR(objects[uarmh->otyp]), "aksil-hukumat dubulg'a") ) ) {
			copcnt = (copcnt / 2) + 1;
		}

		if (RngeAntiGovernment) {
			copcnt = (copcnt / 2) + 1;
		}

	      while(--copcnt >= 0) {
			(void) makemon(mkclass(S_KOP,0), u.ux, u.uy, MM_ANGRY);

			if (!rn2(10)) {

				int koptryct = 0;
				int kox, koy;

				for (koptryct = 0; koptryct < 2000; koptryct++) {
					kox = rn1(COLNO-3,2);
					koy = rn2(ROWNO);

					if (kox && koy && isok(kox, koy) && (levl[kox][koy].typ > DBWALL) && !(t_at(kox, koy)) ) {
						(void) maketrap(kox, koy, KOP_CUBE, 0);
						break;
						}
				}

			}

		} /* while */

	}

      if(mtmp->data == &mons[PM_MEDUSA] && !u.medusaremoved) {
		u.medusaremoved = 1;
		u.uhpmax += rnd(20);
		u.uenmax += rnd(20);
		if (Upolyd) u.mhmax += rnd(20);
	}

#ifdef RECORD_ACHIEVE
        if(mtmp->data == &mons[PM_MEDUSA]) {

		if (!achieve.killed_medusa) {

			if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "team splat cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "vosklitsatel'nyy znak plashch komanda") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "jamoasi xavfsizlik plash") )) pline("TROPHY GET!");
			if (RngeTeamSplat) pline("TROPHY GET!");

			if (uarmc && uarmc->oartifact == ART_JUNETHACK______WINNER) {
				u.uhpmax += 10;
				u.uenmax += 10;
				if (Upolyd) u.mhmax += 10;
				pline("Well done! Your maximum health and mana were increased to make sure you'll get even more trophies! Go for it!");
			}

		}

            achieve.killed_medusa = 1;
	}
        if(mtmp->data == &mons[PM_NIGHTMARE]) {

		if (!achieve.killed_nightmare) {

			if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "team splat cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "vosklitsatel'nyy znak plashch komanda") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "jamoasi xavfsizlik plash") )) pline("TROPHY GET!");
			if (RngeTeamSplat) pline("TROPHY GET!");

			if (uarmc && uarmc->oartifact == ART_JUNETHACK______WINNER) {
				u.uhpmax += 10;
				u.uenmax += 10;
				if (Upolyd) u.mhmax += 10;
				pline("Well done! Your maximum health and mana were increased to make sure you'll get even more trophies! Go for it!");
			}

		}

            achieve.killed_nightmare = 1;

	}
        if(mtmp->data == &mons[PM_VECNA]) {

		if (!achieve.killed_vecna) {

			if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "team splat cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "vosklitsatel'nyy znak plashch komanda") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "jamoasi xavfsizlik plash") )) pline("TROPHY GET!");
			if (RngeTeamSplat) pline("TROPHY GET!");

			if (uarmc && uarmc->oartifact == ART_JUNETHACK______WINNER) {
				u.uhpmax += 10;
				u.uenmax += 10;
				if (Upolyd) u.mhmax += 10;
				pline("Well done! Your maximum health and mana were increased to make sure you'll get even more trophies! Go for it!");
			}

		}

            achieve.killed_vecna = 1;
	}
        if(mtmp->data == &mons[PM_BEHOLDER]) {

		if (!achieve.killed_beholder) {

			if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "team splat cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "vosklitsatel'nyy znak plashch komanda") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "jamoasi xavfsizlik plash") )) pline("TROPHY GET!");
			if (RngeTeamSplat) pline("TROPHY GET!");

			if (uarmc && uarmc->oartifact == ART_JUNETHACK______WINNER) {
				u.uhpmax += 10;
				u.uenmax += 10;
				if (Upolyd) u.mhmax += 10;
				pline("Well done! Your maximum health and mana were increased to make sure you'll get even more trophies! Go for it!");
			}

		}

            achieve.killed_beholder = 1;
	}
        if(mtmp->data == &mons[PM_RUGGO_THE_GNOME_KING]) {

		if (!achieve.killed_ruggo) {

			if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "team splat cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "vosklitsatel'nyy znak plashch komanda") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "jamoasi xavfsizlik plash") )) pline("TROPHY GET!");
			if (RngeTeamSplat) pline("TROPHY GET!");

			if (uarmc && uarmc->oartifact == ART_JUNETHACK______WINNER) {
				u.uhpmax += 10;
				u.uenmax += 10;
				if (Upolyd) u.mhmax += 10;
				pline("Well done! Your maximum health and mana were increased to make sure you'll get even more trophies! Go for it!");
			}

		}

            achieve.killed_ruggo = 1;
	}
        if(mtmp->data == &mons[PM_KROO_THE_KOBOLD_KING]) {

		if (!achieve.killed_kroo) {

			if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "team splat cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "vosklitsatel'nyy znak plashch komanda") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "jamoasi xavfsizlik plash") )) pline("TROPHY GET!");
			if (RngeTeamSplat) pline("TROPHY GET!");

			if (uarmc && uarmc->oartifact == ART_JUNETHACK______WINNER) {
				u.uhpmax += 10;
				u.uenmax += 10;
				if (Upolyd) u.mhmax += 10;
				pline("Well done! Your maximum health and mana were increased to make sure you'll get even more trophies! Go for it!");
			}

		}

            achieve.killed_kroo = 1;
	}
        if(mtmp->data == &mons[PM_GRUND_THE_ORC_KING]) {

		if (!achieve.killed_grund) {

			if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "team splat cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "vosklitsatel'nyy znak plashch komanda") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "jamoasi xavfsizlik plash") )) pline("TROPHY GET!");
			if (RngeTeamSplat) pline("TROPHY GET!");

			if (uarmc && uarmc->oartifact == ART_JUNETHACK______WINNER) {
				u.uhpmax += 10;
				u.uenmax += 10;
				if (Upolyd) u.mhmax += 10;
				pline("Well done! Your maximum health and mana were increased to make sure you'll get even more trophies! Go for it!");
			}

		}

            achieve.killed_grund = 1;
	}
        if(mtmp->data == &mons[PM_THE_LARGEST_GIANT]) {

		if (!achieve.killed_largestgiant) {

			if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "team splat cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "vosklitsatel'nyy znak plashch komanda") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "jamoasi xavfsizlik plash") )) pline("TROPHY GET!");
			if (RngeTeamSplat) pline("TROPHY GET!");

			if (uarmc && uarmc->oartifact == ART_JUNETHACK______WINNER) {
				u.uhpmax += 10;
				u.uenmax += 10;
				if (Upolyd) u.mhmax += 10;
				pline("Well done! Your maximum health and mana were increased to make sure you'll get even more trophies! Go for it!");
			}

		}

            achieve.killed_largestgiant = 1;
	}
        if(mtmp->data == &mons[PM_SHELOB]) {

		if (!achieve.killed_shelob) {

			if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "team splat cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "vosklitsatel'nyy znak plashch komanda") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "jamoasi xavfsizlik plash") )) pline("TROPHY GET!");
			if (RngeTeamSplat) pline("TROPHY GET!");

			if (uarmc && uarmc->oartifact == ART_JUNETHACK______WINNER) {
				u.uhpmax += 10;
				u.uenmax += 10;
				if (Upolyd) u.mhmax += 10;
				pline("Well done! Your maximum health and mana were increased to make sure you'll get even more trophies! Go for it!");
			}

		}

            achieve.killed_shelob = 1;
	}
        if(mtmp->data == &mons[PM_GIRTAB]) {

		if (!achieve.killed_girtab) {

			if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "team splat cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "vosklitsatel'nyy znak plashch komanda") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "jamoasi xavfsizlik plash") )) pline("TROPHY GET!");
			if (RngeTeamSplat) pline("TROPHY GET!");

			if (uarmc && uarmc->oartifact == ART_JUNETHACK______WINNER) {
				u.uhpmax += 10;
				u.uenmax += 10;
				if (Upolyd) u.mhmax += 10;
				pline("Well done! Your maximum health and mana were increased to make sure you'll get even more trophies! Go for it!");
			}

		}

            achieve.killed_girtab = 1;
	}
        if(mtmp->data == &mons[PM_APHRODITE]) {

		if (!achieve.killed_aphrodite) {

			if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "team splat cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "vosklitsatel'nyy znak plashch komanda") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "jamoasi xavfsizlik plash") )) pline("TROPHY GET!");
			if (RngeTeamSplat) pline("TROPHY GET!");

			if (uarmc && uarmc->oartifact == ART_JUNETHACK______WINNER) {
				u.uhpmax += 10;
				u.uenmax += 10;
				if (Upolyd) u.mhmax += 10;
				pline("Well done! Your maximum health and mana were increased to make sure you'll get even more trophies! Go for it!");
			}

		}

            achieve.killed_aphrodite = 1;
	}
        if(mtmp->data == &mons[PM_DOCTOR_FRANKENSTEIN]) {

		if (!achieve.killed_frankenstein) {

			if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "team splat cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "vosklitsatel'nyy znak plashch komanda") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "jamoasi xavfsizlik plash") )) pline("TROPHY GET!");
			if (RngeTeamSplat) pline("TROPHY GET!");

			if (uarmc && uarmc->oartifact == ART_JUNETHACK______WINNER) {
				u.uhpmax += 10;
				u.uenmax += 10;
				if (Upolyd) u.mhmax += 10;
				pline("Well done! Your maximum health and mana were increased to make sure you'll get even more trophies! Go for it!");
			}

		}

            achieve.killed_frankenstein = 1;
	}
        if(mtmp->data == &mons[PM_CROESUS]) {

		if (!achieve.killed_croesus) {

			if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "team splat cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "vosklitsatel'nyy znak plashch komanda") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "jamoasi xavfsizlik plash") )) pline("TROPHY GET!");
			if (RngeTeamSplat) pline("TROPHY GET!");

			if (uarmc && uarmc->oartifact == ART_JUNETHACK______WINNER) {
				u.uhpmax += 10;
				u.uenmax += 10;
				if (Upolyd) u.mhmax += 10;
				pline("Well done! Your maximum health and mana were increased to make sure you'll get even more trophies! Go for it!");
			}

		}

            achieve.killed_croesus = 1;
	}
        if(mtmp->data == &mons[PM_FATHER_DAGON]) {

		if (!achieve.killed_dagon) {

			if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "team splat cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "vosklitsatel'nyy znak plashch komanda") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "jamoasi xavfsizlik plash") )) pline("TROPHY GET!");
			if (RngeTeamSplat) pline("TROPHY GET!");

			if (uarmc && uarmc->oartifact == ART_JUNETHACK______WINNER) {
				u.uhpmax += 10;
				u.uenmax += 10;
				if (Upolyd) u.mhmax += 10;
				pline("Well done! Your maximum health and mana were increased to make sure you'll get even more trophies! Go for it!");
			}

		}

            achieve.killed_dagon = 1;
	}
        if(mtmp->data == &mons[PM_MOTHER_HYDRA]) {

		if (!achieve.killed_hydra) {

			if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "team splat cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "vosklitsatel'nyy znak plashch komanda") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "jamoasi xavfsizlik plash") )) pline("TROPHY GET!");
			if (RngeTeamSplat) pline("TROPHY GET!");

			if (uarmc && uarmc->oartifact == ART_JUNETHACK______WINNER) {
				u.uhpmax += 10;
				u.uenmax += 10;
				if (Upolyd) u.mhmax += 10;
				pline("Well done! Your maximum health and mana were increased to make sure you'll get even more trophies! Go for it!");
			}

		}

            achieve.killed_hydra = 1;
	}
#ifdef LIVELOGFILE
		livelog_achieve_update();
#endif
#endif

	if(memory_is_invisible(mtmp->mx, mtmp->my))
	    unmap_object(mtmp->mx, mtmp->my);
	m_detach(mtmp, mptr);
}

/* TRUE if corpse might be dropped, magr may die if mon was swallowed */
boolean
corpse_chance(mon, magr, was_swallowed)
struct monst *mon;
struct monst *magr;			/* killer, if swallowed */
boolean was_swallowed;			/* digestion */
{
	struct permonst *mdat = mon->data;
	int i, tmp;

	if (mdat == &mons[PM_VLAD_THE_IMPALER] || mdat->mlet == S_LICH) {
	    if (cansee(mon->mx, mon->my) && !was_swallowed)
		pline("%s body crumbles into dust.", s_suffix(Monnam(mon)));
		if (PlayerHearsSoundEffects) pline(issoviet ? "Vy ne poluchite trup. Ya udivlen, chto vy dazhe udalos' pobedit' takogo monstra, dolzhno byt', udacha, potomu chto ty na samom dele ochen' plokhoy igrok..." : "Dueouaaaaaaaaaah.");
	    /* KMH -- make_corpse() handles Vecna */
	    return (mdat == &mons[PM_VECNA]);
	}

	/* Gas spores always explode upon death */
	for(i = 0; i < NATTK; i++) {
	    if (mdat->mattk[i].aatyp == AT_BOOM) {
	    	if (mdat->mattk[i].damn)
	    	    tmp = d((int)mdat->mattk[i].damn,
	    	    		(int)mdat->mattk[i].damd);
	    	else if(mdat->mattk[i].damd)
	    	    tmp = d((int)mdat->mlevel+1, (int)mdat->mattk[i].damd);
	    	else tmp = 0;
		if (was_swallowed && magr) {
		    if (magr == &youmonst) {
			There("is an explosion in your %s!",
			      body_part(STOMACH));
			sprintf(killer_buf, "%s explosion",
				s_suffix(mdat->mname));
			if (Half_physical_damage && rn2(2) ) tmp = (tmp+1) / 2;
			losehp(tmp, killer_buf, KILLED_BY_AN);
		    } else {
			if (flags.soundok) You_hear("an explosion.");
			magr->mhp -= tmp;
			if (magr->mhp < 1) mondied(magr);
			if (magr->mhp < 1) { /* maybe lifesaved */
			    if (canspotmon(magr))
				pline("%s rips open!", Monnam(magr));
			} else if (canseemon(magr))
			    pline("%s seems to have indigestion.",
				  Monnam(magr));
		    }

		    return FALSE;
		}

	    	sprintf(killer_buf, "%s explosion", s_suffix(mdat->mname));
	    	killer = killer_buf;
	    	killer_format = KILLED_BY_AN;
	    	explode(mon->mx, mon->my, -1, tmp, MON_EXPLODE, EXPL_NOXIOUS); 
	    	return (FALSE);
	    } else if (mon->egotype_exploder) {
		tmp = d(2, 1 + (mon->m_lev * 5) );
		if (was_swallowed && magr) {
		    if (magr == &youmonst) {
			There("is an explosion in your %s!",
			      body_part(STOMACH));
			sprintf(killer_buf, "%s explosion",
				s_suffix(mdat->mname));
			if (Half_physical_damage && rn2(2) ) tmp = (tmp+1) / 2;
			losehp(tmp, killer_buf, KILLED_BY_AN);
		    } else {
			if (flags.soundok) You_hear("an explosion.");
			magr->mhp -= tmp;
			if (magr->mhp < 1) mondied(magr);
			if (magr->mhp < 1) { /* maybe lifesaved */
			    if (canspotmon(magr))
				pline("%s rips open!", Monnam(magr));
			} else if (canseemon(magr))
			    pline("%s seems to have indigestion.",
				  Monnam(magr));
		    }

		    return FALSE;
		}

	    	sprintf(killer_buf, "%s explosion", s_suffix(mdat->mname));
	    	killer = killer_buf;
	    	killer_format = KILLED_BY_AN;
	    	explode(mon->mx, mon->my, -1, tmp, MON_EXPLODE, EXPL_NOXIOUS); 
	    	return (FALSE);
	    }
  	}

	/* Cthulhu Deliquesces... */
	if (mdat == &mons[PM_CTHULHU]) {
	    if (cansee(mon->mx, mon->my))
		pline("%s body deliquesces into a cloud of noxious gas!",
			s_suffix(Monnam(mon)));
	    else
		You_hear("hissing and bubbling!");
	    /* ...into a stinking cloud... */
	    (void) create_cthulhu_death_cloud(mon->mx, mon->my, 3, 8);
	    return (FALSE);
	}

	/* must duplicate this below check in xkilled() since it results in
	 * creating no objects as well as no corpse
	 */
	if (LEVEL_SPECIFIC_NOCORPSE(mdat))
		return FALSE;

	if ( (u.uprops[STARVATION_EFFECT].extrinsic || StarvationEffect || (uarmc && uarmc->oartifact == ART_FEMMY_FATALE) || have_starvationstone() ) && STARVATION_SPECIFIC_NOCORPSE(mdat))
		return FALSE;

	/* generally lower chance to leave corpses for balancing reasons, but only if the player is advanced enough --Amy */
	if (!rn2(3) && !((u.urexp < 10000) && (moves < 10000)) && !mon->egotype_troll && !(is_reviver(mdat) && !(mdat->mlet == S_FUNGUS) ) && !(mdat == &mons[PM_TROLL_ZOMBIE]) && !(mdat == &mons[PM_VECNA]) && !(mdat == &mons[PM_NIGHTMARE]) && !(mdat == &mons[PM_BEHOLDER]) && !(mdat == &mons[PM_MEDUSA]) && !(mdat == &mons[PM_EGO_TROLL_MUMMY]) && !(mdat == &mons[PM_TROLL_PERMAMIMIC_MUMMY]) && !(mdat == &mons[PM_TROLL_MUMMY]) && !mon->mtame)
		return FALSE;

	/* make it even less likely later in the game, because monsters are spawning more often anyway */
	if (!timebasedlowerchance() && !timebasedlowerchance() && !is_reviver(mdat) && !mon->egotype_troll && !(mdat == &mons[PM_TROLL_ZOMBIE]) && !(mdat == &mons[PM_VECNA]) && !(mdat == &mons[PM_NIGHTMARE]) && !(mdat == &mons[PM_BEHOLDER]) && !(mdat == &mons[PM_MEDUSA]) && !(mdat == &mons[PM_EGO_TROLL_MUMMY]) && !(mdat == &mons[PM_TROLL_PERMAMIMIC_MUMMY]) && !(mdat == &mons[PM_TROLL_MUMMY]) && !mon->mtame)
		return FALSE;

	if (mon->egotype_troll || (is_reviver(mdat) && !(mdat->mlet == S_FUNGUS) ) || bigmonst(mdat) || mdat == &mons[PM_LIZARD] || mdat == &mons[PM_CAVE_LIZARD] || mdat == &mons[PM_CHAOS_LIZARD] || mdat == &mons[PM_CHAOTIC_LIZARD] || mdat == &mons[PM_LIZARD_EEL] || mdat == &mons[PM_EEL_LIZARD] || mdat == &mons[PM_BLUE_LIZARD] || mdat == &mons[PM_GRASS_LIZARD] || mdat == &mons[PM_SWAMP_LIZARD] || mdat == &mons[PM_SPITTING_LIZARD] || mdat == &mons[PM_ROCK_LIZARD] || mdat == &mons[PM_BABY_CAVE_LIZARD] || mdat == &mons[PM_NIGHT_LIZARD] || mdat == &mons[PM_LIZARD_MAN] || mdat == &mons[PM_LIZARD_KING] || mdat == &mons[PM_LIZARD_OF_YENDOR] || mdat == &mons[PM_ANTI_STONE_LIZARD]  || mdat == &mons[PM_GIANT_LIZARD] || mdat == &mons[PM_HIDDEN_LIZARD] || mdat == &mons[PM_DEFORMED_LIZARD] || mdat == &mons[PM_MIMIC_LIZARD] || mdat == &mons[PM_CLINGING_LIZARD] || mdat == &mons[PM_HUGE_LIZARD] || mdat == &mons[PM_PREHISTORIC_CAVE_LIZARD] || mdat == &mons[PM_KARMIC_LIZARD] || mdat == &mons[PM_GREEN_LIZARD] || mdat == &mons[PM_SAND_TIDE] || mdat == &mons[PM_MONSTER_LIZARD] || mdat == &mons[PM_FBI_AGENT] || mdat == &mons[PM_OWN_SMOKE] || mdat == &mons[PM_GRANDPA] || mdat == &mons[PM_FIRE_LIZARD] || mdat == &mons[PM_LIGHTNING_LIZARD] || mdat == &mons[PM_ICE_LIZARD] || mdat == &mons[PM_BLACK_LIZARD] || mdat == &mons[PM_KATNISS]
		   || is_golem(mdat)
		   || is_mplayer(mdat)
		   || is_umplayer(mdat)
		   || is_deadlysin(mdat)
		   || is_rider(mdat))
		return TRUE;

	return (boolean) (!rn2((int) /* why the heck does frequency matter for this??? --Amy */
		(2 /*+ ((int)(mdat->geno & G_FREQ)<2)*/ + verysmall(mdat))));
}

/* drop (perhaps) a cadaver and remove monster */
void
mondied(mdef)
register struct monst *mdef;
{
	mondead(mdef);
	if (mdef->mhp > 0) return;	/* lifesaved */

	if (corpse_chance(mdef, (struct monst *)0, FALSE) /*&&
	    (accessible(mdef->mx, mdef->my) || is_waterypool(mdef->mx, mdef->my))*/)
		(void) make_corpse(mdef);
}

/* monster disappears, not dies */
void
mongone(mdef)
register struct monst *mdef;
{
	mdef->mhp = 0;	/* can skip some inventory bookkeeping */
	/* Player is thrown from his steed when it disappears */
	if (mdef == u.usteed)
		dismount_steed(DISMOUNT_GENERIC);

	/* drop special items like the Amulet so that a dismissed Kop or nurse
	   can't remove them from the game */
	mdrop_special_objs(mdef);
	/* release rest of monster's inventory--it is removed from game */
	discard_minvent(mdef);
#ifndef GOLDOBJ
	mdef->mgold = 0L;
#endif
	m_detach(mdef, mdef->data);
}

/* drop a statue or rock and remove monster */
void
monstone(mdef)
register struct monst *mdef;
{
	struct obj *otmp, *obj, *oldminvent;
	xchar x = mdef->mx, y = mdef->my;
	boolean wasinside = FALSE;

	/* we have to make the statue before calling mondead, to be able to
	 * put inventory in it, and we have to check for lifesaving before
	 * making the statue....
	 */
	lifesaved_monster(mdef);
	if (mdef->mhp > 0) return;

	mdef->mtrapped = 0;	/* (see m_detach) */

	if ((int)mdef->data->msize > MZ_TINY ||
		    !rn2(2 /*+ ((int) (mdef->data->geno & G_FREQ) > 2) */ )) {
		oldminvent = 0;
		/* some objects may end up outside the statue */
		while ((obj = mdef->minvent) != 0) {
		    obj_extract_self(obj);
		    if (obj->owornmask)
			update_mon_intrinsics(mdef, obj, FALSE, TRUE);
		    obj_no_longer_held(obj);
		    if (obj->owornmask & W_WEP)
			setmnotwielded(mdef,obj);
		    obj->owornmask = 0L;
		    if (obj->otyp == BOULDER ||
#if 0				/* monsters don't carry statues */
     (obj->otyp == STATUE && mons[obj->corpsenm].msize >= mdef->data->msize) ||
#endif
				obj_resists(obj, 0, 0)) {
			if (flooreffects(obj, x, y, "fall")) continue;
			place_object(obj, x, y);
		    } else {
			if (obj->lamplit) end_burn(obj, TRUE);
			obj->nobj = oldminvent;
			oldminvent = obj;
		    }
		}
		/* defer statue creation until after inventory removal
		   so that saved monster traits won't retain any stale
		   item-conferred attributes */
		otmp = mkcorpstat(STATUE, KEEPTRAITS(mdef) ? mdef : 0,
				  mdef->data, x, y, FALSE);
		if (mdef->mnamelth) otmp = oname(otmp, NAME(mdef));
		while ((obj = oldminvent) != 0) {
		    oldminvent = obj->nobj;

		/* reduce amount of musable items the player can use --Amy */
		if (is_musable(obj) && obj->mstartinvent && !(obj->oartifact) && !(obj->enchantment) && !(obj->fakeartifact) && !stack_too_big(obj) && (!rn2(3) || (rn2(100) < u.musableremovechance) || LootcutBug || u.uprops[LOOTCUT_BUG].extrinsic || have_lootcutstone() || !timebasedlowerchance() ) && !(mdef->data == &mons[PM_GOOD_ITEM_MASTER]) && !(mdef->data == &mons[PM_BAD_ITEM_MASTER]) ) delobj(obj);
		else if (obj->mstartinventB && !(obj->oartifact) && !(obj->enchantment) && !(obj->fakeartifact) && !stack_too_big(obj) && (!rn2(4) || (rn2(100) < u.equipmentremovechance) || !timebasedlowerchance() ) && !(mdef->data == &mons[PM_GOOD_ITEM_MASTER]) && !(mdef->data == &mons[PM_BAD_ITEM_MASTER]) ) delobj(obj);
		    else (void) add_to_container(otmp, obj);
		}
#ifndef GOLDOBJ
		if (mdef->mgold) {
			struct obj *au;
			au = mksobj(GOLD_PIECE, FALSE, FALSE);
			au->quan = mdef->mgold;
			au->owt = weight(au);
			(void) add_to_container(otmp, au);
			mdef->mgold = 0;
		}
#endif
		/* Archeologists should not break unique statues */
		if (mdef->data->geno & G_UNIQ)
			otmp->spe = 1;
		otmp->owt = weight(otmp);
	} else
		otmp = mksobj_at(ROCK, x, y, TRUE, FALSE);

	if (otmp) stackobj(otmp);
	/* mondead() already does this, but we must do it before the newsym */
	if(memory_is_invisible(x, y))
	    unmap_object(x, y);
	if (cansee(x, y)) newsym(x,y);
	/* We don't currently trap the hero in the statue in this case but we could */
	if (u.uswallow && u.ustuck == mdef) wasinside = TRUE;
	(void) stop_timer(UNPOLY_MON, (void *) mdef);
	mondead(mdef);
	if (wasinside) {
		if (is_animal(mdef->data))
			You("%s through an opening in the new %s.",
				locomotion(youmonst.data, "jump"),
				otmp ? xname(otmp) : "item");
	}
}

/* another monster has killed the monster mdef */
void
monkilled(mdef, fltxt, how)
register struct monst *mdef;
const char *fltxt;
int how;
{
	boolean be_sad = FALSE;		/* true if unseen pet is killed */

	if ((mdef->wormno ? worm_known(mdef) : cansee(mdef->mx, mdef->my))
		&& fltxt)
	    pline("%s is %s%s%s!", Monnam(mdef),
			nonliving(mdef->data) ? "destroyed" : "killed",
		    *fltxt ? " by the " : "",
		    fltxt
		 );
	else
	    be_sad = (mdef->mtame != 0 && !mdef->isspell);

	if (mdef->mtame != 0 && uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "poke mongo cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "sovat' mongo plashch") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "soktudun mongo plash") ) ) {
		pline("You allowed a pet to die, thereby incurring the wrath of the gods!");
		u.ugangr += rnd(3);
	}

	/* no corpses if digested or disintegrated */
	if(how == AD_DGST || how == -AD_RBRE)
	    mondead(mdef);
	else
	    mondied(mdef);

	if (be_sad && mdef->mhp <= 0) {
	    You(Hallucination ? "are feeling totally down for a moment, then it passes." : (Role_if(PM_PIRATE) || Role_if(PM_KORSAIR) || (uwep && uwep->oartifact == ART_ARRRRRR_MATEY) ) ? "hang the jib for a moment, then it passes." : "have a sad feeling for a moment, then it passes.");
	    if (PlayerHearsSoundEffects) pline(issoviet ? "Da! Vash pitomets umer! Tip bloka l'da ochen' pozabavilo!" : "Daeaeae-aeaeaeaeae!");
	}

}


/* WAC -- another monster has killed the monster mdef and you get exp. */
void
mon_xkilled(mdef, fltxt, how)
	register struct monst *mdef;
	const char *fltxt;
	int how;
{
	boolean be_sad = FALSE;         /* true if unseen pet is killed */

	if ((mdef->wormno ? worm_known(mdef) : cansee(mdef->mx, mdef->my))
			&& fltxt)
		pline("%s is %s%s%s!", Monnam(mdef),
				nonliving(mdef->data) ? "destroyed" : "killed",
				*fltxt ? " by the " : "",
				fltxt);
	else
		be_sad = (mdef->mtame != 0 && !mdef->isspell); 

	/* no corpses if digested or disintegrated */
	if(how == AD_DGST || how == -AD_RBRE)
		xkilled(mdef,2);
	else
		xkilled(mdef,0);

	if (be_sad && mdef->mhp <= 0) {
	    You(Hallucination ? "are feeling totally down for a moment, then it passes." : (Role_if(PM_PIRATE) || Role_if(PM_KORSAIR) || (uwep && uwep->oartifact == ART_ARRRRRR_MATEY) ) ? "hang the jib for a moment, then it passes." : "have a sad feeling for a moment, then it passes.");
	    if (PlayerHearsSoundEffects) pline(issoviet ? "Da! Vash pitomets umer! Tip bloka l'da ochen' pozabavilo!" : "Daeaeae-aeaeaeaeae!");
	}
}


void
unstuck(mtmp)
register struct monst *mtmp;
{
	if(u.ustuck == mtmp) {
		if(u.uswallow){
			u.ux = mtmp->mx;
			u.uy = mtmp->my;
			u.uswallow = 0;
			u.uswldtim = 0;
			if (Punished) placebc();
			vision_full_recalc = 1;
			docrt();
		}
		setustuck(0);
	}
}

void
killed(mtmp)
register struct monst *mtmp;
{
	xkilled(mtmp, 1);
}

/* the player has killed the monster mtmp */
void
xkilled(mtmp, dest)
	register struct monst *mtmp;
/*
 * Dest=1, normal; dest=0, don't print message; dest=2, don't drop corpse
 * either; dest=3, message but no corpse
 */
	int	dest;
{
	register int tmp, x = mtmp->mx, y = mtmp->my;
	register struct permonst *mdat;
	int mndx;
	register struct obj *otmp;
	register struct trap *t;
	boolean redisp = FALSE;
	boolean wasinside = u.uswallow && (u.ustuck == mtmp);


	/* KMH, conduct */
	u.uconduct.killer++;

	if (Role_if(PM_BLOODSEEKER)) healup(mtmp->m_lev, 0, FALSE, FALSE); /* special ability called "Stygwyr's Thirst" */

	if (uarmf && OBJ_DESCR(objects[uarmf->otyp]) && ( !strcmp(OBJ_DESCR(objects[uarmf->otyp]), "red sneakers") || !strcmp(OBJ_DESCR(objects[uarmf->otyp]), "krasnyye krossovki") || !strcmp(OBJ_DESCR(objects[uarmf->otyp]), "qizil shippak") )) healup( (mtmp->m_lev / 3), 0, FALSE, FALSE);

	if (Manaleech && !rn2(3) ) { /* leech mana from killed monsters */
		u.uen += rno(mtmp->m_lev + 1); /* rno instead of rnd, and added rn2 above, due to this property being too unbalanced --Amy */
		if (u.uen > u.uenmax) u.uen = u.uenmax;
	}

	if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "energizer cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "antidepressant plashch") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "energiya plash")) && !rn2(3) ) {
		u.uen += rno((mtmp->m_lev + 5) / 5);
		if (u.uen > u.uenmax) u.uen = u.uenmax;

	}

	if (dest & 1) {
	    const char *verb = nonliving(mtmp->data) ? "destroy" : "kill";

	    if ((!wasinside && !canspotmon(mtmp)) || (mtmp->data == &mons[PM__S_RAT_MAN]) || (mtmp->data == &mons[PM__S_SECRET_CAR]) || (mtmp->data == &mons[PM__S_UFO]) || (mtmp->data == &mons[PM__S_____NIX]) )
		You("%s it!", verb);
	    else {
		You("%s %s!", verb,
		    !mtmp->mtame ? mon_nam(mtmp) :
			x_monnam(mtmp,
				 mtmp->mnamelth ? ARTICLE_NONE : ARTICLE_THE,
				 "poor",
				 mtmp->mnamelth ? SUPPRESS_SADDLE : 0,
				 FALSE));
	    }
	}

	if (mtmp->mtrapped && (t = t_at(x, y)) != 0 &&
		(t->ttyp == PIT || t->ttyp == SPIKED_PIT || t->ttyp == GIANT_CHASM || t->ttyp == SHIT_PIT || t->ttyp == MANA_PIT || t->ttyp == ANOXIC_PIT) &&
		sobj_at(BOULDER, x, y))
	    dest |= 2;     /*
			    * Prevent corpses/treasure being created "on top"
			    * of the boulder that is about to fall in. This is
			    * out of order, but cannot be helped unless this
			    * whole routine is rearranged.
			    */

	/* your pet knows who just killed it...watch out */
	if (mtmp->mtame && !mtmp->isminion) EDOG(mtmp)->killed_by_u = 1;

	/* dispose of monster and make cadaver */
	if(stoned) monstone(mtmp);
	else mondead(mtmp);

	if (mtmp->mhp > 0) { /* monster cheated death */
		/* Cannot put the non-visible lifesaving message in
		 * lifesaved_monster()/unpoly_monster() since the message
		 * appears only when you kill it (as opposed to visible
		 * lifesaving which always appears).
		 */
		stoned = FALSE;
		if ((!u.uswallow || u.ustuck != mtmp) && !cansee(x, y))
		    pline("Maybe not...");
		return;
	}

	mdat = mtmp->data; /* note: mondead can change mtmp->data */
	mndx = monsndx(mdat);

	if (stoned) {
		stoned = FALSE;
		goto cleanup;
	}

	if((dest & 2) || LEVEL_SPECIFIC_NOCORPSE(mdat))
		goto cleanup;

#ifdef MAIL
	if(mdat == &mons[PM_MAIL_DAEMON]) {
		stackobj(mksobj_at(SCR_MAIL, x, y, FALSE, FALSE));
		redisp = TRUE;
	}
#endif

	/* In Soviet Russia, you may certainly never get something from a monster if it dies while you're engulfed by it
	 * or while the monster is in a wall. After all, that would be a violation of the Kreml's regulations. Not that the
	 * items generated that way would do any harm to anyone, but of course "rules are rules"... --Amy */

	if (issoviet && (x == u.ux) && (y == u.uy) ) goto sovietnothing;

	if((!accessible(x, y) && !is_waterypool(x, y) && !is_watertunnel(x,y)) ||
	   (x == u.ux && y == u.uy)) {
	    /* might be mimic in wall or corpse in lava or on player's spot */
	    redisp = TRUE;
	    if(wasinside) spoteffects(TRUE);
	    if (issoviet) goto sovietnothing;
	} /*else*/ if(/*x != u.ux || y != u.uy && */!attacktype(mdat, AT_MULTIPLY) && (!mtmp->isspell) && (!mtmp->egotype_multiplicator) ) { /* multipliers could otherwise be farmed */
		/* might be here after swallowed */

		/* Throw a bone to vampiric and ghast players who cannot unstone themselves easily. --Amy */
		if ((mdat == &mons[PM_LIZARD] || mdat == &mons[PM_CAVE_LIZARD] || mdat == &mons[PM_PREHISTORIC_CAVE_LIZARD] || mdat == &mons[PM_CHAOS_LIZARD] || mdat == &mons[PM_HUGE_LIZARD] || mdat == &mons[PM_CHAOTIC_LIZARD] || mdat == &mons[PM_SAND_TIDE] || mdat == &mons[PM_FIRE_LIZARD] || mdat == &mons[PM_ROCK_LIZARD] || mdat == &mons[PM_BABY_CAVE_LIZARD] || mdat == &mons[PM_NIGHT_LIZARD] || mdat == &mons[PM_FBI_AGENT] || mdat == &mons[PM_OWN_SMOKE] || mdat == &mons[PM_GRANDPA] || mdat == &mons[PM_LIGHTNING_LIZARD] || mdat == &mons[PM_KARMIC_LIZARD] || mdat == &mons[PM_GREEN_LIZARD] || mdat == &mons[PM_BLACK_LIZARD] || mdat == &mons[PM_MONSTER_LIZARD] || mdat == &mons[PM_ICE_LIZARD] || mdat == &mons[PM_GRASS_LIZARD] || mdat == &mons[PM_BLUE_LIZARD] || mdat == &mons[PM_SWAMP_LIZARD] || mdat == &mons[PM_SPITTING_LIZARD] || mdat == &mons[PM_LIZARD_EEL] || mdat == &mons[PM_HIDDEN_LIZARD] || mdat == &mons[PM_DEFORMED_LIZARD] || mdat == &mons[PM_MIMIC_LIZARD] || mdat == &mons[PM_CLINGING_LIZARD] || mdat == &mons[PM_LIZARD_MAN] || mdat == &mons[PM_LIZARD_OF_YENDOR] || mdat == &mons[PM_LIZARD_KING] || mdat == &mons[PM_GIANT_LIZARD] || mdat == &mons[PM_EEL_LIZARD] || mdat == &mons[PM_ANTI_STONE_LIZARD]) && !rn2(5) && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && timebasedlowerchance() && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(POT_ACID, x, y, TRUE, FALSE);
		/* of course the acid potions are useful for other races too, if they run out of lizard corpses */

		if ((mdat == &mons[PM_SQUIRREL] || mdat == &mons[PM_IGUANA] || mdat == &mons[PM_HELPFUL_SQUIRREL] || mdat == &mons[PM_BIG_IGUANA]) && !rn2(20) && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && timebasedlowerchance() && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(CARROT, x, y, TRUE, FALSE);
		if ((mdat == &mons[PM_SQUIRREL] || mdat == &mons[PM_IGUANA] || mdat == &mons[PM_HELPFUL_SQUIRREL] || mdat == &mons[PM_BIG_IGUANA]) && !rn2(20) && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && timebasedlowerchance() && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(BANANA, x, y, TRUE, FALSE);
		if ((mdat == &mons[PM_SQUIRREL] || mdat == &mons[PM_IGUANA] || mdat == &mons[PM_HELPFUL_SQUIRREL] || mdat == &mons[PM_BIG_IGUANA]) && !rn2(20) && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && timebasedlowerchance() && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(MELON, x, y, TRUE, FALSE);
		if ((mdat == &mons[PM_SQUIRREL] || mdat == &mons[PM_IGUANA] || mdat == &mons[PM_HELPFUL_SQUIRREL] || mdat == &mons[PM_BIG_IGUANA]) && !rn2(20) && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && timebasedlowerchance() && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(PEAR, x, y, TRUE, FALSE);
		if ((mdat == &mons[PM_SQUIRREL] || mdat == &mons[PM_IGUANA] || mdat == &mons[PM_HELPFUL_SQUIRREL] || mdat == &mons[PM_BIG_IGUANA]) && !rn2(50) && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && timebasedlowerchance() && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(ASIAN_PEAR, x, y, TRUE, FALSE);

		if ((mdat == &mons[PM_GECKO] || mdat == &mons[PM_GIANT_GECKO] || mdat == &mons[PM_FLYING_GECKO]) && !rn2(40) && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && timebasedlowerchance() && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(EUCALYPTUS_LEAF, x, y, TRUE, FALSE);

		if ((mdat == &mons[PM_RHAUMBUSUN] || mdat == &mons[PM_BIG_RHAUMBUSUN]) && !rn2(20) && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && timebasedlowerchance() && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(SCR_FIRE, x, y, TRUE, FALSE);

		if ((mdat == &mons[PM_SALAMANDER] || mdat == &mons[PM_FROST_SALAMANDER] || mdat == &mons[PM_KOMODO_DRAGON] || mdat == &mons[PM_PETTY_KOMODO_DRAGON]) && !rn2(20) && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && timebasedlowerchance() && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(CREAM_PIE, x, y, TRUE, FALSE);
		if ((mdat == &mons[PM_SALAMANDER] || mdat == &mons[PM_FROST_SALAMANDER] || mdat == &mons[PM_KOMODO_DRAGON] || mdat == &mons[PM_PETTY_KOMODO_DRAGON]) && !rn2(20) && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && timebasedlowerchance() && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(APPLE, x, y, TRUE, FALSE);
		if ((mdat == &mons[PM_SALAMANDER] || mdat == &mons[PM_FROST_SALAMANDER] || mdat == &mons[PM_KOMODO_DRAGON] || mdat == &mons[PM_PETTY_KOMODO_DRAGON]) && !rn2(20) && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && timebasedlowerchance() && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(ORANGE, x, y, TRUE, FALSE);
		if ((mdat == &mons[PM_SALAMANDER] || mdat == &mons[PM_FROST_SALAMANDER] || mdat == &mons[PM_KOMODO_DRAGON] || mdat == &mons[PM_PETTY_KOMODO_DRAGON]) && !rn2(20) && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && timebasedlowerchance() && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(LEMON, x, y, TRUE, FALSE);

		if (mdat == &mons[PM_SMALL_ITEM_TROVE]) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);

		if (mdat == &mons[PM_ITEM_TROVE]) {
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
		}

		if (mdat == &mons[PM_LARGE_ITEM_TROVE]) {
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
		}

		if (mdat == &mons[PM_GIANT_ITEM_TROVE]) {
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
		}

		if (mdat == &mons[PM_ENORMOUS_ITEM_TROVE]) {
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
		}

		if (mdat == &mons[PM_UNIHORN_TROVE]) otmp = mksobj_at(UNICORN_HORN, x, y, TRUE, FALSE);

		if (mdat == &mons[PM_MEGA_UNIHORN_TROVE]) {
			otmp = mksobj_at(UNICORN_HORN, x, y, TRUE, FALSE);
			otmp = mksobj_at(UNICORN_HORN, x, y, TRUE, FALSE);
		}

		if (mdat == &mons[PM_POTION_TROVE]) otmp = mkobj_at(POTION_CLASS, x, y, TRUE);
		if (mdat == &mons[PM_WEAPON_TROVE]) otmp = mkobj_at(WEAPON_CLASS, x, y, TRUE);
		if (mdat == &mons[PM_ARMOR_TROVE]) otmp = mkobj_at(ARMOR_CLASS, x, y, TRUE);
		if (mdat == &mons[PM_RING_TROVE]) otmp = mkobj_at(RING_CLASS, x, y, TRUE);
		if (mdat == &mons[PM_AMULET_TROVE]) otmp = mkobj_at(AMULET_CLASS, x, y, TRUE);
		if (mdat == &mons[PM_TOOL_TROVE]) otmp = mkobj_at(TOOL_CLASS, x, y, TRUE);
		if (mdat == &mons[PM_FOOD_TROVE]) otmp = mkobj_at(FOOD_CLASS, x, y, TRUE);
		if (mdat == &mons[PM_SCROLL_TROVE]) otmp = mkobj_at(SCROLL_CLASS, x, y, TRUE);
		if (mdat == &mons[PM_SPELLBOOK_TROVE]) otmp = mkobj_at(SPBOOK_CLASS, x, y, TRUE);
		if (mdat == &mons[PM_WAND_TROVE]) otmp = mkobj_at(WAND_CLASS, x, y, TRUE);
		if (mdat == &mons[PM_GEM_TROVE]) otmp = mkobj_at(GEM_CLASS, x, y, TRUE);

		if (mdat == &mons[PM_LARGE_POTION_TROVE]) {
			otmp = mkobj_at(POTION_CLASS, x, y, TRUE);
			otmp = mkobj_at(POTION_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_LARGE_WEAPON_TROVE]) {
			otmp = mkobj_at(WEAPON_CLASS, x, y, TRUE);
			otmp = mkobj_at(WEAPON_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_LARGE_ARMOR_TROVE]) {
			otmp = mkobj_at(ARMOR_CLASS, x, y, TRUE);
			otmp = mkobj_at(ARMOR_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_LARGE_RING_TROVE]) {
			otmp = mkobj_at(RING_CLASS, x, y, TRUE);
			otmp = mkobj_at(RING_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_LARGE_AMULET_TROVE]) {
			otmp = mkobj_at(AMULET_CLASS, x, y, TRUE);
			otmp = mkobj_at(AMULET_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_LARGE_TOOL_TROVE]) {
			otmp = mkobj_at(TOOL_CLASS, x, y, TRUE);
			otmp = mkobj_at(TOOL_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_LARGE_FOOD_TROVE]) {
			otmp = mkobj_at(FOOD_CLASS, x, y, TRUE);
			otmp = mkobj_at(FOOD_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_LARGE_SCROLL_TROVE]) {
			otmp = mkobj_at(SCROLL_CLASS, x, y, TRUE);
			otmp = mkobj_at(SCROLL_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_LARGE_SPELLBOOK_TROVE]) {
			otmp = mkobj_at(SPBOOK_CLASS, x, y, TRUE);
			otmp = mkobj_at(SPBOOK_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_LARGE_WAND_TROVE]) {
			otmp = mkobj_at(WAND_CLASS, x, y, TRUE);
			otmp = mkobj_at(WAND_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_LARGE_GEM_TROVE]) {
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
		}

		if (mdat == &mons[PM_BIG_POTION_TROVE]) {
			otmp = mkobj_at(POTION_CLASS, x, y, TRUE);
			otmp = mkobj_at(POTION_CLASS, x, y, TRUE);
			otmp = mkobj_at(POTION_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_BIG_WEAPON_TROVE]) {
			otmp = mkobj_at(WEAPON_CLASS, x, y, TRUE);
			otmp = mkobj_at(WEAPON_CLASS, x, y, TRUE);
			otmp = mkobj_at(WEAPON_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_BIG_ARMOR_TROVE]) {
			otmp = mkobj_at(ARMOR_CLASS, x, y, TRUE);
			otmp = mkobj_at(ARMOR_CLASS, x, y, TRUE);
			otmp = mkobj_at(ARMOR_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_BIG_RING_TROVE]) {
			otmp = mkobj_at(RING_CLASS, x, y, TRUE);
			otmp = mkobj_at(RING_CLASS, x, y, TRUE);
			otmp = mkobj_at(RING_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_BIG_AMULET_TROVE]) {
			otmp = mkobj_at(AMULET_CLASS, x, y, TRUE);
			otmp = mkobj_at(AMULET_CLASS, x, y, TRUE);
			otmp = mkobj_at(AMULET_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_BIG_TOOL_TROVE]) {
			otmp = mkobj_at(TOOL_CLASS, x, y, TRUE);
			otmp = mkobj_at(TOOL_CLASS, x, y, TRUE);
			otmp = mkobj_at(TOOL_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_BIG_FOOD_TROVE]) {
			otmp = mkobj_at(FOOD_CLASS, x, y, TRUE);
			otmp = mkobj_at(FOOD_CLASS, x, y, TRUE);
			otmp = mkobj_at(FOOD_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_BIG_SCROLL_TROVE]) {
			otmp = mkobj_at(SCROLL_CLASS, x, y, TRUE);
			otmp = mkobj_at(SCROLL_CLASS, x, y, TRUE);
			otmp = mkobj_at(SCROLL_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_BIG_SPELLBOOK_TROVE]) {
			otmp = mkobj_at(SPBOOK_CLASS, x, y, TRUE);
			otmp = mkobj_at(SPBOOK_CLASS, x, y, TRUE);
			otmp = mkobj_at(SPBOOK_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_BIG_WAND_TROVE]) {
			otmp = mkobj_at(WAND_CLASS, x, y, TRUE);
			otmp = mkobj_at(WAND_CLASS, x, y, TRUE);
			otmp = mkobj_at(WAND_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_BIG_GEM_TROVE]) {
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
		}

		if (mdat == &mons[PM_GIANT_POTION_TROVE]) {
			otmp = mkobj_at(POTION_CLASS, x, y, TRUE);
			otmp = mkobj_at(POTION_CLASS, x, y, TRUE);
			otmp = mkobj_at(POTION_CLASS, x, y, TRUE);
			otmp = mkobj_at(POTION_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_GIANT_WEAPON_TROVE]) {
			otmp = mkobj_at(WEAPON_CLASS, x, y, TRUE);
			otmp = mkobj_at(WEAPON_CLASS, x, y, TRUE);
			otmp = mkobj_at(WEAPON_CLASS, x, y, TRUE);
			otmp = mkobj_at(WEAPON_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_GIANT_ARMOR_TROVE]) {
			otmp = mkobj_at(ARMOR_CLASS, x, y, TRUE);
			otmp = mkobj_at(ARMOR_CLASS, x, y, TRUE);
			otmp = mkobj_at(ARMOR_CLASS, x, y, TRUE);
			otmp = mkobj_at(ARMOR_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_GIANT_RING_TROVE]) {
			otmp = mkobj_at(RING_CLASS, x, y, TRUE);
			otmp = mkobj_at(RING_CLASS, x, y, TRUE);
			otmp = mkobj_at(RING_CLASS, x, y, TRUE);
			otmp = mkobj_at(RING_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_GIANT_AMULET_TROVE]) {
			otmp = mkobj_at(AMULET_CLASS, x, y, TRUE);
			otmp = mkobj_at(AMULET_CLASS, x, y, TRUE);
			otmp = mkobj_at(AMULET_CLASS, x, y, TRUE);
			otmp = mkobj_at(AMULET_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_GIANT_TOOL_TROVE]) {
			otmp = mkobj_at(TOOL_CLASS, x, y, TRUE);
			otmp = mkobj_at(TOOL_CLASS, x, y, TRUE);
			otmp = mkobj_at(TOOL_CLASS, x, y, TRUE);
			otmp = mkobj_at(TOOL_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_GIANT_FOOD_TROVE]) {
			otmp = mkobj_at(FOOD_CLASS, x, y, TRUE);
			otmp = mkobj_at(FOOD_CLASS, x, y, TRUE);
			otmp = mkobj_at(FOOD_CLASS, x, y, TRUE);
			otmp = mkobj_at(FOOD_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_GIANT_SCROLL_TROVE]) {
			otmp = mkobj_at(SCROLL_CLASS, x, y, TRUE);
			otmp = mkobj_at(SCROLL_CLASS, x, y, TRUE);
			otmp = mkobj_at(SCROLL_CLASS, x, y, TRUE);
			otmp = mkobj_at(SCROLL_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_GIANT_SPELLBOOK_TROVE]) {
			otmp = mkobj_at(SPBOOK_CLASS, x, y, TRUE);
			otmp = mkobj_at(SPBOOK_CLASS, x, y, TRUE);
			otmp = mkobj_at(SPBOOK_CLASS, x, y, TRUE);
			otmp = mkobj_at(SPBOOK_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_GIANT_WAND_TROVE]) {
			otmp = mkobj_at(WAND_CLASS, x, y, TRUE);
			otmp = mkobj_at(WAND_CLASS, x, y, TRUE);
			otmp = mkobj_at(WAND_CLASS, x, y, TRUE);
			otmp = mkobj_at(WAND_CLASS, x, y, TRUE);
		}
		if (mdat == &mons[PM_GIANT_GEM_TROVE]) {
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
			otmp = mkobj_at(GEM_CLASS, x, y, TRUE);
		}

		if (!rn2(100) && (Race_if(PM_ANGBANDER) || (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "angband cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "plashch sredizem'ye krepost'") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "o'rta yer qal'a plash") ) ) ) && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(SCR_TELEPORTATION, x, y, TRUE, FALSE);
		if (!rn2(100) && Race_if(PM_ANGBANDER) && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(SCR_IDENTIFY, x, y, TRUE, FALSE);
		if (!rn2(100) && RngeAngband && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(SCR_TELEPORTATION, x, y, TRUE, FALSE);

		if (!rn2(500) && timebasedlowerchance() && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(SCR_CURE, x, y, TRUE, FALSE);
		if (!rn2(Race_if(PM_ROHIRRIM) ? 100 : 250) && timebasedlowerchance() && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(SCR_PHASE_DOOR, x, y, TRUE, FALSE);
		if (!rn2(100) && timebasedlowerchance() && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(SCR_MANA, x, y, TRUE, FALSE);
		if (!rn2(120) && timebasedlowerchance() && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(SCR_STANDARD_ID, x, y, TRUE, FALSE);
		if (!rn2(40) && timebasedlowerchance() && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(SCR_HEALING, x, y, TRUE, FALSE);

		if (uarmg && uarmg->oartifact == ART_SCROLLSCROLLSCROLL) {
			if (!rn2(500) && timebasedlowerchance() && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(SCR_CURE, x, y, TRUE, FALSE);
			if (!rn2(250) && timebasedlowerchance() && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(SCR_PHASE_DOOR, x, y, TRUE, FALSE);
			if (!rn2(100) && timebasedlowerchance() && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(SCR_MANA, x, y, TRUE, FALSE);
			if (!rn2(120) && timebasedlowerchance() && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(SCR_STANDARD_ID, x, y, TRUE, FALSE);
			if (!rn2(40) && timebasedlowerchance() && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(SCR_HEALING, x, y, TRUE, FALSE);
		}

		if (!rn2(500) && timebasedlowerchance() && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && (rn2(100) > u.usefulitemchance) ) otmp = mksobj_at(usefulitem(), x, y, TRUE, FALSE);

		/* you should not be able to farm trolls, gremlins, long worms etc. --Amy */
		if (!rn2( (Race_if(PM_DROW) ? 50 : Race_if(PM_DOPPELGANGER) ? 75 : 16) ) && !(u.uprops[NO_DROPS_EFFECT].extrinsic || NoDropsEffect || have_droplessstone() ) && !is_reviver(mdat) && !is_rider(mdat) && !is_deadlysin(mdat) && mdat != &mons[PM_GREMLIN] && mdat != &mons[PM_LONG_WORM] && mdat != &mons[PM_TROLL_ZOMBIE] && mdat != &mons[PM_TROLL_MUMMY] && mdat != &mons[PM_TROLL_PERMAMIMIC_MUMMY] && mdat != &mons[PM_EGO_TROLL_MUMMY] && timebasedlowerchance() && (rn2(100) > u.usefulitemchance) && !(issoviet && (mvitals[mndx].mvflags & G_NOCORPSE)) && !(issoviet && nohands(mdat))
	/* lowered overall chance, but see below for a chance to get extra items --Amy
	 * Drow and especially Doppelgangers are super-powerful anyway, so I decided to nerf them a bit. */
					&& (!issoviet || (mdat->mlet != S_KOP))
							) { /* allow death drops for every monster type --Amy */
			int typ;

			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(32) && (rn2(100) > u.usefulitemchance) ) {
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE); /* small chance to get even more stuff --Amy */
			}
			if (!rn2(96) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) ) {
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE); /* small chance to get even more stuff --Amy */
			if (!rn2(3)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			}
			if (!rn2(288) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) ) {
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE); /* small chance to get even more stuff --Amy */
			if (!rn2(3)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(9)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			}
			if (!rn2(864) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) ) {
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(3)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE); /* small chance to get even more stuff --Amy */
			if (!rn2(9)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(27)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			}
			if (!rn2(2592) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) ) {
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(3)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE); /* small chance to get even more stuff --Amy */
			if (!rn2(9)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(27)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(81)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			}
			if (!rn2(7776) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) ) {
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(3)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE); /* small chance to get even more stuff --Amy */
			if (!rn2(9)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(27)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(81)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(243)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			}
			if (!rn2(23328) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) ) {
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(3)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE); /* small chance to get even more stuff --Amy */
			if (!rn2(9)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(27)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(81)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(243)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(729)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			}
			if (!rn2(69984) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) ) {
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(3)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE); /* small chance to get even more stuff --Amy */
			if (!rn2(9)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(27)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(81)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(243)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(729)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(2187)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			}
			if (!rn2(209952) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) ) {
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(3)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE); /* small chance to get even more stuff --Amy */
			if (!rn2(9)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(27)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(81)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(243)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(729)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(2187)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(6561)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			}
			if (!rn2(629856) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) && (rn2(100) > u.usefulitemchance) ) {
			otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(3)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE); /* small chance to get even more stuff --Amy */
			if (!rn2(9)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(27)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(81)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(243)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(729)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(2187)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(6561)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			if (!rn2(19683)) otmp = mkobj_at(RANDOM_CLASS, x, y, TRUE);
			}

			/* Don't create large objects from small monsters */
			/*if (otmp) typ = otmp->otyp;*/
			/*if (mdat->msize < MZ_HUMAN && typ != FOOD_RATION 
			    && typ != LEATHER_LEASH
			    && typ != INKA_LEASH
			    && typ != FIGURINE
			    && (otmp->owt > 3 ||
				objects[typ].oc_big oc_bimanual/oc_bulky ||
				is_spear(otmp) || is_pole(otmp) ||
				typ == MORNING_STAR)) {
			    delobj(otmp);
			} else */

			redisp = TRUE; /*just always create the object --Amy*/
		}
		/* Whether or not it always makes a corpse is, in theory,
		 * different from whether or not the corpse is "special";
		 * if we want both, we have to specify it explicitly.
		 */
		if (corpse_chance(mtmp, (struct monst *)0, FALSE))
			(void) make_corpse(mtmp);
	}

sovietnothing:

	/*if(redisp)*/ newsym(x,y);
cleanup:
	/* punish bad behaviour */

	if ( Role_if(PM_LADIESMAN) && !flags.female && mtmp->female && humanoid(mtmp->data) ) {

	    You(Hallucination ? "feel very bad for killing your future wife." : "feel very bad for killing a defenseless woman.");
	    adjalign(-25);
		u.ualign.sins++; 
		u.alignlim--;
		change_luck(-1);

	}

	/* Batman may not kill women, but you can be a female batman and it would be unfair to only punish males. --Amy */
	if ( Race_if(PM_BATMAN) && ((!flags.female && mtmp->female) || (flags.female && !mtmp->female)) && humanoid(mtmp->data)) {

	    Hallucination ? You_feel("very bad for killing your future %s.", flags.female ? "husband" : "wife") : You_feel("very bad for killing a defenseless %s.", flags.female ? "man" : "woman");
	    adjalign(-25);
		u.ualign.sins++; 
		u.alignlim--;
		change_luck(-1);

	}

	if(is_human(mdat) && (!always_hostile(mdat) && mtmp->malign <= 0) &&
	   (mndx < PM_ARCHEOLOGIST || mndx > PM_WIZARD) &&
	   u.ualign.type != A_CHAOTIC) {

		if (always_peaceful(mdat)) { /* being penalized for killing maia, imperials etc. was just stupid. --Amy */
		HTelepat &= ~INTRINSIC;
		change_luck( u.ualign.type == A_LAWFUL ? -2 : -1); /* lower penalty for neutrals --Amy */
		You(Hallucination ? "killed someone you weren't supposed to - whoops!" : "murderer!");

		if (Role_if(PM_PALADIN)) { /* more severe murderer penalties */
			u.ualign.sins += 5; 
			u.alignlim -= 5;
			change_luck(-1);
			adjalign(-50);
		}

		if(u.ualign.type == A_LAWFUL) { u.ualign.sins += 3; u.alignlim -= 3;} /*fall through*/
		u.ualign.sins += 2; 
		u.alignlim -= 2;
		if (Blind && !Blind_telepat)
		    see_monsters(); /* Can't sense monsters any more. */
		}
		adjalign(u.ualign.type == A_LAWFUL ? -2 : -1);

	}
	if((mtmp->mpeaceful && !rn2(2)) || mtmp->mtame)	change_luck(-1);
	if (is_unicorn(mdat) &&
				sgn(u.ualign.type) == sgn(mdat->maligntyp)) {
		change_luck(-5);
		You_feel("guilty...");
	}

	if ( (Role_if(PM_ACTIVISTOR) || Race_if(PM_PEACEMAKER)) && mdat == &mons[PM_TOPMODEL]) { /* very bad idea! --Amy */
		You_feel("guilty for killing an innocent girl.");
		change_luck(-5);
		u.ualign.sins += 10; 
		u.alignlim -= 10;
		adjalign(-50);
		u.ugangr++; u.ugangr++; u.ugangr++;
		prayer_done();

	}
	/* give experience points */
	tmp = experience(mtmp, (int)mvitals[mndx].died + 1);
	more_experienced(tmp, 0);
	newexplevel();		/* will decide if you go up */

	if (!rn2(19150)) { /* evil patch idea by jonadab: summon a boss */
		pline("The RNG decides to send in reinforcements to punish people who kill innocent monsters.");

		int attempts = 0;
		register struct permonst *ptrZ;

		if (Aggravate_monster) {
			u.aggravation = 1;
			reset_rndmonst(NON_PM);
		}

newbossA:
		do {
			ptrZ = rndmonst();
			attempts++;
			if (!rn2(2000)) reset_rndmonst(NON_PM);

		} while ( (!ptrZ || (ptrZ && !(ptrZ->geno & G_UNIQ))) && attempts < 50000);

		if (ptrZ && ptrZ->geno & G_UNIQ) {
			if (wizard) pline("monster generation: %s", ptrZ->mname);
			(void) makemon(ptrZ, u.ux, u.uy, NO_MM_FLAGS);
		}
		else if (rn2(50)) {
			attempts = 0;
			goto newbossA;
		}

		u.aggravation = 0;

	}

	if (mdat->maligntyp > 0 && is_nethermist(mtmp->mx, mtmp->my) ) {
		levl[mtmp->mx][mtmp->my].typ = CORR;
		if (cansee(mtmp->mx,mtmp->my)) {
			pline("The nether mist dissipates.");
			newsym(mtmp->mx, mtmp->my);
		}
	}

	/* adjust alignment points */
	if (mtmp->m_id == quest_status.leader_m_id) {		/* REAL BAD! */
	    adjalign(-(u.ualign.record+u.alignlim/2));
	    pline("That was %sa bad idea...",
	    		u.uevent.qcompleted ? "probably " : "");
	} else if (mdat->msound == MS_NEMESIS)	/* Real good! */
	    adjalign((u.alignlim/4));
	else if (mdat->msound == MS_GUARDIAN) {	/* Bad */
	    adjalign(-(u.alignlim/8));
	    if (!Hallucination) pline("That was probably a bad idea...");
	    else pline("Whoopsie-daisy!");
	} else if (mtmp->ispriest) {
		adjalign((p_coaligned(mtmp)) ? -2 : 2);
		/* cancel divine protection for killing your priest */
		if (p_coaligned(mtmp)) u.ublessed = 0;
		if (mdat->maligntyp == A_NONE)
			adjalign(u.alignlim / 4);		/* BIG bonus */
	} else if (mtmp->mtame) {
		adjalign(-50);	/* bad!! */
		/* your god is mighty displeased... */
		if (!Hallucination) {(Role_if(PM_PIRATE) || Role_if(PM_KORSAIR) || (uwep && uwep->oartifact == ART_ARRRRRR_MATEY) ) ? pline("Batten down the hatches!") : You_hear("the rumble of distant thunder...");}
		else You_hear("the studio audience applaud!");
		if (PlayerHearsSoundEffects) pline(issoviet ? "Molodets, geroy - ty ubil sobstvennogo domashnego zhivotnogo, potomu chto vy byli glupy. Vy na samom dele sovetskaya Pyat' Lo? Potomu chto on ne igrayet namnogo khuzhe, chem vy." : "Wummm. Wummmmmmmm!");

		if (uarmc && OBJ_DESCR(objects[uarmc->otyp]) && (!strcmp(OBJ_DESCR(objects[uarmc->otyp]), "poke mongo cloak") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "sovat' mongo plashch") || !strcmp(OBJ_DESCR(objects[uarmc->otyp]), "soktudun mongo plash") ) ) {
			pline("You killed your pet, thereby incurring the wrath of the gods!");
			u.ugangr += rnd(3);
		}

	} else if (mtmp->mpeaceful) {
		adjalign(-15);
		if (!Hallucination) pline("The gods will probably not appreciate this...");
		else pline("Whoopsie-daisy!");
	}

	/* malign was already adjusted for u.ualign.type and randomization */
	adjalign(mtmp->malign);

#ifdef LIVELOG_BONES_KILLER 
	livelog_bones_killed(mtmp); 
#endif 
}

/* changes the monster into a stone monster of the same type */
/* this should only be called when poly_when_stoned() is true */
void
mon_to_stone(mtmp)
    register struct monst *mtmp;
{
    boolean polymorphed = mtmp->oldmonnm != monsndx(mtmp->data);

    if(mtmp->data->mlet == S_GOLEM) {
	/* it's a golem, and not a stone golem */
	if(canseemon(mtmp))
	    pline("%s solidifies...", Monnam(mtmp));
	if (newcham(mtmp, &mons[PM_STONE_GOLEM], FALSE, FALSE)) {
	    if (!polymorphed)
		mtmp->oldmonnm = PM_STONE_GOLEM;    /* Change is permanent */
	    if(canseemon(mtmp))
		pline("Now it's %s.", an(mtmp->data->mname));
	} else {
	    if(canseemon(mtmp))
		pline("... and returns to normal.");
	}
    } else
	impossible("Can't polystone %s!", a_monnam(mtmp));
}

void
mnexto(mtmp)	/* Make monster mtmp next to you (if possible) */
	struct monst *mtmp;
{
	coord mm;

	if (mtmp == u.usteed) {
		/* Keep your steed in sync with you instead */
		mtmp->mx = u.ux;
		mtmp->my = u.uy;
		return;
	}

	if(!enexto(&mm, u.ux, u.uy, mtmp->data)) return;
	rloc_to(mtmp, mm.x, mm.y);
	return;
}

/* mnearto()
 * Put monster near (or at) location if possible.
 * Returns:
 *	1 - if a monster was moved from x, y to put mtmp at x, y.
 *	0 - in most cases.
 */
boolean
mnearto(mtmp,x,y,move_other)
register struct monst *mtmp;
xchar x, y;
boolean move_other;	/* make sure mtmp gets to x, y! so move m_at(x, y) */
{
	struct monst *othermon = (struct monst *)0;
	xchar newx, newy;
	coord mm;

	if ((mtmp->mx == x) && (mtmp->my == y)) return(FALSE);

	if (move_other && (othermon = m_at(x, y))) {
		if (othermon->wormno)
			remove_worm(othermon);
		else
			remove_monster(x, y);
	}

	newx = x;
	newy = y;

	if (!goodpos(newx, newy, mtmp, 0)) {
		/* actually we have real problems if enexto ever fails.
		 * migrating_mons that need to be placed will cause
		 * no end of trouble.
		 */
		if (!enexto(&mm, newx, newy, mtmp->data)) return(FALSE);
		newx = mm.x; newy = mm.y;
	}

	rloc_to(mtmp, newx, newy);

	if (move_other && othermon) {
	    othermon->mx = othermon->my = 0;
	    (void) mnearto(othermon, x, y, FALSE);
	    if ((othermon->mx != x) || (othermon->my != y))
		return(TRUE);
	}

	return(FALSE);
}


static const char *poiseff[] = {

	" feel weaker", "r brain is on fire",
	"r judgement is impaired", "r muscles won't obey you",
	" feel very sick", " break out in hives"
};

void
poisontell(typ)

	int	typ;
{
	pline("You%s.", poiseff[typ]);
	pline("You lose  %s", typ == 0 ? "Strength" : typ == 1 ? "Intelligence" : typ == 2 ? "Wisdom" : typ == 3 ? "Dexterity" : typ == 4 ? "Constitution" : "Charisma");
}

void
poisoned(string, typ, pname, fatal)
const char *string, *pname;
int  typ, fatal;
{
	int i, plural, kprefix = KILLED_BY_AN;
	boolean thrown_weapon = (fatal < 0);

	if (thrown_weapon) fatal = -fatal;
	if(strcmp(string, "blast") && !thrown_weapon) {
	    /* 'blast' has already given a 'poison gas' message */
	    /* so have "poison arrow", "poison dart", etc... */
	    plural = (string[strlen(string) - 1] == 's')? 1 : 0;
	    /* avoid "The" Orcus's sting was poisoned... */
	    pline("%s%s %s poisoned!", isupper((int)*string) ? "" : "The ",
			string, plural ? "were" : "was");
	}

	if(Poison_resistance && rn2(20) ) {
		if(!strcmp(string, "blast")) shieldeff(u.ux, u.uy);
		pline_The("poison doesn't seem to affect you.");

		if(!rn2(20)) {
		/* Check that a stat change was made */
		if (adjattrib(typ, -1, 1)) {
		    pline("You%s!", poiseff[typ]);
			pline("You lose  %s", typ == 0 ? "Strength" : typ == 1 ? "Intelligence" : typ == 2 ? "Wisdom" : typ == 3 ? "Dexterity" : typ == 4 ? "Constitution" : "Charisma");			 } 
			}
		return;
	}
	/* suppress killer prefix if it already has one */
	if ((i = name_to_mon(pname)) >= LOW_PM && mons[i].geno & G_UNIQ) {
	    kprefix = KILLED_BY;
	    if (!type_is_pname(&mons[i])) pname = the(pname);
	} else if (!strncmpi(pname, "the ", 4) ||
	    !strncmpi(pname, "an ", 3) ||
	    !strncmpi(pname, "a ", 2)) {
	    /*[ does this need a plural check too? ]*/
	    kprefix = KILLED_BY;
	}
	i = rn2(fatal + 20*thrown_weapon);
	if(i == 0 && !Poison_resistance && !(uarms && uarms->oartifact == ART_ANTINSTANT_DEATH) && typ != A_CHA && !rn2(100)) {
		if (Invulnerable || (Stoned_chiller && Stoned))
		   pline("You are unharmed!");
		else {
		u.uhp = -1;
		pline_The("poison was deadly...");
		}
	} else if(i <= 5) {
		/* Check that a stat change was made */
		if (adjattrib(typ, thrown_weapon ? -1 : -rn1(3,3), 1)) {
		    pline("You%s!", poiseff[typ]);
			pline("You lose  %s", typ == 0 ? "Strength" : typ == 1 ? "Intelligence" : typ == 2 ? "Wisdom" : typ == 3 ? "Dexterity" : typ == 4 ? "Constitution" : "Charisma");
		}

		/* still does damage --Amy */
		i = thrown_weapon ? rnd(6) : rn1(10,6);
		if(Half_physical_damage && rn2(2) ) i = (i+1) / 2;
		losehp(i, pname, kprefix);

	} else {
		i = thrown_weapon ? rnd(6) : rn1(10,6);
		if(Half_physical_damage && rn2(2) ) i = (i+1) / 2;
		losehp(i, pname, kprefix);
	}
	if(u.uhp < 1) {
		u.youaredead = 1;
		killer_format = kprefix;
		killer = pname;
		/* "Poisoned by a poisoned ___" is redundant */
		done(strstri(pname, "poison") ? DIED : POISONING);
		u.youaredead = 0;
	}
	(void) encumber_msg();
}

/* monster responds to player action; not the same as a passive attack */
/* assumes reason for response has been tested, and response _must_ be made */
void
m_respond(mtmp)
register struct monst *mtmp;
{
    if(mtmp->data->msound == MS_SHRIEK) {
	if(flags.soundok) {
	    pline("%s shrieks.", Monnam(mtmp));
	    stop_occupation();
	}

	if (Aggravate_monster) {
		u.aggravation = 1;
		reset_rndmonst(NON_PM);
	}

	/* [Tom] took out the weird purple worm thing and lowered prob from 10 */        
	if (!rn2(8)) {
/*          if (!rn2(13))
		(void) makemon(&mons[PM_PURPLE_WORM], 0, 0, NO_MM_FLAGS);
	    else  */
		(void) makemon((struct permonst *)0, 0, 0, NO_MM_FLAGS);

	}

	u.aggravation = 0;

	aggravate();
    }
    if(!mtmp->egotype_farter && mtmp->data->msound == MS_FART_QUIET) {
		pline("%s produces %s farting noises with %s %s butt.", Monnam(mtmp), rn2(2) ? "tender" : "soft", mhis(mtmp), mtmp->female ? "sexy" : "ugly" );
		if (uarmf && uarmf->oartifact == ART_ELIANE_S_SHIN_SMASH) {
			pline("The farting gas destroys your footwear instantly.");
		      useup(uarmf);
		}
		if (mtmp->butthurt) mtmp->butthurt--;
		if (mtmp->butthurt) mtmp->butthurt--;
		if (mtmp->butthurt) mtmp->butthurt--;
		if (uarmf && uarmf->oartifact == ART_ELIANE_S_COMBAT_SNEAKERS) {
			pline("Eek! You can't stand farting gas!");
			badeffect();
			badeffect();
			badeffect();
			badeffect();
		}
		badeffect();
    }
    if(!mtmp->egotype_farter && mtmp->data->msound == MS_FART_NORMAL) {
		pline("%s produces %s farting noises with %s %s butt.", Monnam(mtmp), rn2(2) ? "beautiful" : "squeaky", mhis(mtmp), mtmp->female ? "sexy" : "ugly" );
		if (uarmf && uarmf->oartifact == ART_ELIANE_S_SHIN_SMASH) {
			pline("The farting gas destroys your footwear instantly.");
		      useup(uarmf);
		}
		if (mtmp->butthurt) mtmp->butthurt--;
		if (mtmp->butthurt) mtmp->butthurt--;
		if (uarmf && uarmf->oartifact == ART_ELIANE_S_COMBAT_SNEAKERS) {
			pline("Eek! You can't stand farting gas!");
			badeffect();
			badeffect();
			badeffect();
			badeffect();
		}
		badeffect();
    }
    if(!mtmp->egotype_farter && mtmp->data->msound == MS_FART_LOUD) {
		pline("%s produces %s farting noises with %s %s butt.", Monnam(mtmp), rn2(2) ? "disgusting" : "loud", mhis(mtmp), mtmp->female ? "sexy" : "ugly" );
		if (uarmf && uarmf->oartifact == ART_ELIANE_S_SHIN_SMASH) {
			pline("The farting gas destroys your footwear instantly.");
		      useup(uarmf);
		}
		if (mtmp->butthurt) mtmp->butthurt--;
		if (uarmf && uarmf->oartifact == ART_ELIANE_S_COMBAT_SNEAKERS) {
			pline("Eek! You can't stand farting gas!");
			badeffect();
			badeffect();
			badeffect();
			badeffect();
		}
		badeffect();
    }
    if (mtmp->egotype_farter) {
		pline("%s produces %s farting noises with %s %s butt.", Monnam(mtmp), !rn2(6) ? "disgusting" : !rn2(5) ? "loud" : !rn2(4) ? "tender" : !rn2(3) ? "soft" : !rn2(2) ? "beautiful" : "squeaky", mhis(mtmp), mtmp->female ? "sexy" : "ugly" );
		if (uarmf && uarmf->oartifact == ART_ELIANE_S_SHIN_SMASH) {
			pline("The farting gas destroys your footwear instantly.");
		      useup(uarmf);
		}
		if (uarmf && uarmf->oartifact == ART_ELIANE_S_COMBAT_SNEAKERS) {
			pline("Eek! You can't stand farting gas!");
			badeffect();
			badeffect();
			badeffect();
			badeffect();
		}
		badeffect();
    }

    if(mtmp->data->msound == MS_SOUND || mtmp->egotype_sounder) {
		pline("%s lets out an ear-splitting scream!", Monnam(mtmp) );
		make_stunned(HStun + (mtmp->m_lev + 2), TRUE);
		if (!rn2(issoviet ? 2 : 5)) (void)destroy_item(POTION_CLASS, AD_COLD);
		wake_nearby();
    }

    if(mtmp->data == &mons[PM_MEDUSA]) {
	register int i;
	for(i = 0; i < NATTK; i++)
	     if(mtmp->data->mattk[i].aatyp == AT_GAZE) {
		 (void) gazemu(mtmp, &mtmp->data->mattk[i]);
		 break;
	     }
    }
}

#endif /* OVLB */
#ifdef OVL2

void
setmangry(mtmp)
register struct monst *mtmp;
{
	struct permonst *mdat;
	mdat = mtmp->data;

	mtmp->mstrategy &= ~STRAT_WAITMASK;
	/* Even if the black marketeer is already angry he may not have called
	 * for his assistants if he or his staff have not been assaulted yet.
	 */
	if (Is_blackmarket(&u.uz) && !mtmp->mpeaceful && mtmp->isshk)
	    blkmar_guards(mtmp);
	if(!mtmp->mpeaceful) return;
	if(mtmp->mtame) return;
	mtmp->mpeaceful = 0;

	if (mtmp->isshk || mtmp->ispriest || mtmp->isgd || (mdat == &mons[PM_WATCHMAN]) || (mdat == &mons[PM_WATCH_CAPTAIN]) || (mdat == &mons[PM_WATCH_LEADER]) || (mdat == &mons[PM_WATCH_LIEUTENANT])) {


		int koptryct;
		int kox, koy;

newkopcube:
		koptryct = 0;

		for (koptryct = 0; koptryct < 2000; koptryct++) {
			kox = rn1(COLNO-3,2);
			koy = rn2(ROWNO);

			if (kox && koy && isok(kox, koy) && (levl[kox][koy].typ > DBWALL) && !(t_at(kox, koy)) ) {
				(void) maketrap(kox, koy, KOP_CUBE, 0);
				break;
				}
		}
		if (rn2(2)) goto newkopcube;

	}

	if(mtmp->ispriest) {
		if(p_coaligned(mtmp)) adjalign(-50); /* very bad */
		else adjalign(2);
	} else {
		adjalign(-5);		/* attacking peaceful monsters is bad */
	}

	if (mtmp->isshk && !strncmpi(shkname(mtmp), "Izchak", 6) ) {

		adjalign(-50);

	}

	if (Role_if(PM_PALADIN)) adjalign(-20);
	if (Role_if(PM_PALADIN) && mtmp->isshk && !strncmpi(shkname(mtmp), "Izchak", 6) ) {

	/* Attacking Izchak is grounds for immediate disintegration. */

		adjalign(-200);

		You_feel("the air around you grow charged...");
		pline("Suddenly, you realize that %s has noticed you...", u_gname());
		/* Throw everything we have at the player */
		god_zaps_you(u.ualign.type);

	}

	if (couldsee(mtmp->mx, mtmp->my)) {
		if (humanoid(mtmp->data) || mtmp->isshk || mtmp->isgd)
		    pline("%s gets angry!", Monnam(mtmp));
		else if (flags.verbose && flags.soundok) growl(mtmp);
	}

	/* Don't misbehave in the Black Market or else... */
	if (Is_blackmarket(&u.uz)) {
	    if (mtmp->isshk)
		blkmar_guards(mtmp);
	    else if (NAME(mtmp) && *NAME(mtmp)) {
		/* non-tame named monsters are presumably
		 * black marketeer's assistants */
		struct monst *shkp;
		shkp = shop_keeper(inside_shop(mtmp->mx, mtmp->my));
		if (shkp)  wakeup(shkp);
	    }
	}

	/* attacking your own quest leader will anger his or her guardians */
	if (!flags.mon_moving &&	/* should always be the case here */
		mtmp->data == &mons[quest_info(MS_LEADER)]) {
	    struct monst *mon;
	    struct permonst *q_guardian = &mons[quest_info(MS_GUARDIAN)];
	    int got_mad = 0;

	    /* guardians will sense this attack even if they can't see it */
	    for (mon = fmon; mon; mon = mon->nmon)
		if (!DEADMONSTER(mon) && mon->data == q_guardian && mon->mpeaceful) {
		    mon->mpeaceful = 0;
		    if (canseemon(mon)) ++got_mad;
		}
	    if (got_mad && !Hallucination)
		pline_The("%s appear%s to be angry too...",
		      got_mad == 1 ? q_guardian->mname :
				    makeplural(q_guardian->mname),
		      got_mad == 1 ? "s" : "");
	}
}

void
wakeup(mtmp)
register struct monst *mtmp;
{
	mtmp->msleeping = 0;
	mtmp->meating = 0;	/* assume there's no salvagable food left */
	setmangry(mtmp);
	if(mtmp->m_ap_type) seemimic(mtmp);
	else if (flags.forcefight && !flags.mon_moving && mtmp->mundetected) {
	    mtmp->mundetected = 0;
	    newsym(mtmp->mx, mtmp->my);
	}
}

/* Wake up nearby monsters. */
void
wake_nearby()
{
	register struct monst *mtmp;

	for(mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
	    if (!DEADMONSTER(mtmp) && distu(mtmp->mx,mtmp->my) < level_difficulty()*20) {
		mtmp->msleeping = 0;
		if (mtmp->mtame && !mtmp->isminion)
		    EDOG(mtmp)->whistletime = moves;
	    }
	}

	if (!rn2(250)) for(mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
	    if (!DEADMONSTER(mtmp)) {
		mtmp->msleeping = 0;
		if (mtmp->mtame && !mtmp->isminion)
		    EDOG(mtmp)->whistletime = moves;
	    }
	}

}

/* Wake up monsters near some particular location. */
void
wake_nearto(x, y, distance)
register int x, y, distance;
{
	register struct monst *mtmp;

	for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
	    if (!DEADMONSTER(mtmp) && mtmp->msleeping && (distance == 0 ||
				 dist2(mtmp->mx, mtmp->my, x, y) < distance))
		mtmp->msleeping = 0;
	}
}

/* NOTE: we must check for mimicry before calling this routine */
void
seemimic(mtmp)
register struct monst *mtmp;
{

	/* can't uncover certain types of monsters --Amy */
	if (permamimic(mtmp->data) || mtmp->egotype_permamimic ) return;

	unsigned old_app = mtmp->mappearance;
	uchar old_ap_type = mtmp->m_ap_type;

	mtmp->m_ap_type = M_AP_NOTHING;
	mtmp->mappearance = 0;

	/*
	 *  Discovered mimics don't block light.
	 */
	if (((old_ap_type == M_AP_FURNITURE &&
	      (old_app == S_hcdoor || old_app == S_vcdoor)) ||
	     (old_ap_type == M_AP_OBJECT && old_app == BOULDER)) &&
	    !does_block(mtmp->mx, mtmp->my, &levl[mtmp->mx][mtmp->my]))
	    unblock_point(mtmp->mx, mtmp->my);

	newsym(mtmp->mx,mtmp->my);
}

/* force all chameleons to become normal */
void
rescham()
{
	register struct monst *mtmp;
	int mcham;

	for(mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
		if (DEADMONSTER(mtmp)) continue;
		if (permamimic(mtmp->data) || mtmp->egotype_permamimic ) continue;
		mcham = (int) mtmp->cham;
		if (mcham) {
			mtmp->cham = CHAM_ORDINARY;
			(void) newcham(mtmp, &mons[cham_to_pm[mcham]], FALSE,
				canseemon(mtmp));
		}
		if(is_were(mtmp->data) && mtmp->data->mlet != S_HUMAN)
			new_were(mtmp);
		if(mtmp->m_ap_type && cansee(mtmp->mx, mtmp->my)) {
			seemimic(mtmp);
			/* we pretend that the mimic doesn't */
			/* know that it has been unmasked.   */
			mtmp->msleeping = 1;
		}
	}
}

/* Let the chameleons change again -dgk */
void
restartcham()
{
	register struct monst *mtmp;

	for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
		if (DEADMONSTER(mtmp)) continue;
		mtmp->cham = pm_to_cham(monsndx(mtmp->data));
		if((mtmp->data->mlet == S_MIMIC || standardmimic(mtmp->data) || mtmp->egotype_mimic ) && mtmp->msleeping &&
				cansee(mtmp->mx, mtmp->my)) {
			set_mimic_sym(mtmp);
			newsym(mtmp->mx,mtmp->my);
		}
	}
}

/* called when restoring a monster from a saved level; protection
   against shape-changing might be different now than it was at the
   time the level was saved. */
void
restore_cham(mon)
struct monst *mon;
{
	int mcham;

	if (Protection_from_shape_changers) {
	    mcham = (int) mon->cham;
	    if (mcham) {
		mon->cham = CHAM_ORDINARY;
		(void) newcham(mon, &mons[cham_to_pm[mcham]], FALSE, FALSE);
	    } else if (is_were(mon->data) && !is_human(mon->data)) {
		new_were(mon);
	    }
	} else if (mon->cham == CHAM_ORDINARY) {
	    mon->cham = pm_to_cham(monsndx(mon->data));
	}
}

/* unwatched hiders may hide again; if so, a 1 is returned.  */
STATIC_OVL boolean
restrap(mtmp)
register struct monst *mtmp;
{
	if(mtmp->cham || mtmp->mcan || mtmp->m_ap_type ||
	   cansee(mtmp->mx, mtmp->my) || rn2(3) || (mtmp == u.ustuck) ||
	   (sensemon(mtmp) && distu(mtmp->mx, mtmp->my) <= 2))
		return(FALSE);

	if(mtmp->data->mlet == S_MIMIC || standardmimic(mtmp->data) || mtmp->egotype_mimic ) {
		set_mimic_sym(mtmp);
		return(TRUE);
	} else /* allow piercers and suchlike to hide on all terrain types --Amy */
	    /*if(levl[mtmp->mx][mtmp->my].typ == ROOM)*/  {
		mtmp->mundetected = 1;
		return(TRUE);
	    }

	return(FALSE);
}

short *animal_list = 0;		/* list of PM values for animal monsters */
int animal_list_count;

void
mon_animal_list(construct)
boolean construct;
{
	if (construct) {
	    short animal_temp[SPECIAL_PM];
	    int i, n;

	 /* if (animal_list) impossible("animal_list already exists"); */

	    for (n = 0, i = LOW_PM; i < SPECIAL_PM; i++)
		if (is_animal(&mons[i])) animal_temp[n++] = i;
	 /* if (n == 0) animal_temp[n++] = NON_PM; */

	    animal_list = (short *)alloc(n * sizeof *animal_list);
	    (void) memcpy((void *)animal_list,
			  (void *)animal_temp,
			  n * sizeof *animal_list);
	    animal_list_count = n;
	} else {	/* release */
	    if (animal_list) free((void *)animal_list), animal_list = 0;
	    animal_list_count = 0;
	}
}

STATIC_OVL int
pick_animal()
{
	if (!animal_list) mon_animal_list(TRUE);

	return animal_list[rn2(animal_list_count)];
}

STATIC_OVL int
select_newcham_form(mon)
struct monst *mon;
{
	int mndx = NON_PM;
	struct permonst *pm;

	switch (mon->cham) {
	    case CHAM_SANDESTIN:
	    case CHAM_CHAOS_SHAPECHANGER:
	    case CHAM_DOPPLEZON:
		if (!rn2(7)) {
sandestinchoice:
			mndx = pick_nasty();
			pm = &mons[mndx];
			if (rnd(pm->mlevel + 1) > (mon->m_lev + 10) ) goto sandestinchoice;
			if (uncommon2(pm) && !rn2(4)) goto sandestinchoice;
			if (uncommon3(pm) && !rn2(3)) goto sandestinchoice;
			if (uncommon5(pm) && !rn2(2)) goto sandestinchoice;
			if (uncommon7(pm) && rn2(3)) goto sandestinchoice;
			if (uncommon10(pm) && rn2(5)) goto sandestinchoice;
		}
		break;
	    case CHAM_WARPER:
		if (!rn2(7)) {
warperchoice:
			mndx = rn2(NUMMONS);
			pm = &mons[mndx];
			if (rnd(pm->mlevel + 1) > (mon->m_lev + 10) ) goto warperchoice;
			if (uncommon2(pm) && !rn2(4)) goto warperchoice;
			if (uncommon3(pm) && !rn2(3)) goto warperchoice;
			if (uncommon5(pm) && !rn2(2)) goto warperchoice;
			if (uncommon7(pm) && rn2(3)) goto warperchoice;
			if (uncommon10(pm) && rn2(5)) goto warperchoice;
		}
		break;
	    case CHAM_DOPPELGANGER:
	    case CHAM_MISSINGNO:
	    case CHAM_TRANSFORMER:
		if (!rn2(20)) mndx = pick_nasty();
		else if (!rn2(7)) mndx = rn1(PM_WIZARD - PM_ARCHEOLOGIST + 1,
					    PM_ARCHEOLOGIST);
		break;
	    case CHAM_CHAMELEON:
	    case CHAM_GIANT_CHAMELEON:
		if (!rn2(7)) {
chameleonchoice:
			mndx = pick_animal();
			pm = &mons[mndx];
			if (rnd(pm->mlevel + 1) > (mon->m_lev + 10) ) goto chameleonchoice;
			if (uncommon2(pm) && !rn2(4)) goto chameleonchoice;
			if (uncommon3(pm) && !rn2(3)) goto chameleonchoice;
			if (uncommon5(pm) && !rn2(2)) goto chameleonchoice;
			if (uncommon7(pm) && rn2(3)) goto chameleonchoice;
			if (uncommon10(pm) && rn2(5)) goto chameleonchoice;
		}

		break;
	    case CHAM_ORDINARY:
	      {
		struct obj *m_armr = which_armor(mon, W_ARM);

		if (m_armr && Is_dragon_scales(m_armr))
		    mndx = Dragon_scales_to_pm(m_armr) - mons;
		else if (m_armr && Is_dragon_mail(m_armr))
		    mndx = Dragon_mail_to_pm(m_armr) - mons;
	      }
		break;
	}
#ifdef WIZARD
	/* For debugging only: allow control of polymorphed monster; not saved */
	if (wizard && iflags.mon_polycontrol) {
		char pprompt[BUFSZ], buf[BUFSZ];
		int tries = 0;
		do {
			sprintf(pprompt,
				"Change %s into what kind of monster? [type the name]",
				mon_nam(mon));
			getlin(pprompt,buf);
			mndx = name_to_mon(buf);
			if (mndx < LOW_PM)
				You("cannot polymorph %s into that.", mon_nam(mon));
			else break;
		} while(++tries < 5);
		if (tries==5) pline(thats_enough_tries);
	}
#endif /*WIZARD*/
	if (mndx == NON_PM) {
findrandomform:
		mndx = rn1(SPECIAL_PM - LOW_PM, LOW_PM);
		pm = &mons[mndx];
		if (rnd(pm->mlevel + 1) > (mon->m_lev + 10) ) goto findrandomform;
		if (uncommon2(pm) && !rn2(4)) goto findrandomform;
		if (uncommon3(pm) && !rn2(3)) goto findrandomform;
		if (uncommon5(pm) && !rn2(2)) goto findrandomform;
		if (uncommon7(pm) && rn2(3)) goto findrandomform;
		if (uncommon10(pm) && rn2(5)) goto findrandomform;
	}
	return mndx;
}


/* make a chameleon look like a new monster; returns 1 if it actually changed */
/* [ALI] Special case: Don't print a message if hero can neither spot the
 * original _or_ the new monster (avoids "It turns into it!").
 */
int
newcham(mtmp, mdat, polyspot, msg)
struct monst *mtmp;
struct permonst *mdat;
boolean polyspot;	/* change is the result of wand or spell of polymorph */
boolean msg;
{
	int mhp, hpn, hpd;
	int mndx, tryct;
	int couldsee = canseemon(mtmp);
	struct permonst *olddata = mtmp->data;
	char oldname[BUFSZ];
	boolean alt_mesg = FALSE;	/* Avoid "<rank> turns into a <rank>" */

	if (msg) {
	    /* like Monnam() but never mention saddle */
	    strcpy(oldname, x_monnam(mtmp, ARTICLE_THE, (char *)0,
				     SUPPRESS_SADDLE, FALSE));
	    oldname[0] = highc(oldname[0]);
	}

	/* mdat = 0 -> caller wants a random monster shape */
	tryct = 0;
	if(mdat == 0) {
	    while (++tryct <= 100) {
		mndx = select_newcham_form(mtmp);
		mdat = &mons[mndx];
		if ((mvitals[mndx].mvflags & G_GENOD) != 0 ||
			is_placeholder(mdat)) continue;
		/* polyok rules out all M2_PNAME and M2_WERE's;
		   select_newcham_form might deliberately pick a player
		   character type, so we can't arbitrarily rule out all
		   human forms any more */
		if (is_mplayer(mdat) || is_umplayer(mdat) || (!is_human(mdat) && monpolyok(mdat)))
		    break;
	    }
	    if (tryct > 100) return 0;	/* Should never happen */
	} else if (mvitals[monsndx(mdat)].mvflags & G_GENOD)
	    return(0);	/* passed in mdat is genocided */

	if(is_male(mdat)) {
		if(mtmp->female) mtmp->female = FALSE;
	} else if (is_female(mdat)) {
		if(!mtmp->female) mtmp->female = TRUE;
	} else if (!is_neuter(mdat)) {
		if(!rn2(10)) mtmp->female = !mtmp->female;
	}

	if (In_endgame(&u.uz) && is_mplayer(olddata)) {
		/* mplayers start out as "Foo the Bar", but some of the
		 * titles are inappropriate when polymorphed, particularly
		 * into the opposite sex.  players don't use ranks when
		 * polymorphed, so dropping the rank for mplayers seems
		 * reasonable.
		 */
		char *p = index(NAME(mtmp), ' ');
		if (p) {
			*p = '\0';
			mtmp->mnamelth = p - NAME(mtmp) + 1;
		}
	}

	if(mdat == mtmp->data) return(0);	/* still the same monster */

	/* [ALI] Detect transforming between player monsters with the
	 * same rank title to avoid badly formed messages.
	 * Similarly for were creatures transforming to their alt. form.
	 */
	if (msg && is_mplayer(olddata) && is_mplayer(mdat)) {
	    const struct Role *role;
	    int i, oldmndx;

	    mndx = monsndx(mdat);
	    oldmndx = monsndx(olddata);
	    for (role = roles; role->name.m; role++) {
		if (role->femalenum == NON_PM)
		    continue;
		if ((mndx == role->femalenum && oldmndx == role->malenum) ||
			(mndx == role->malenum && oldmndx == role->femalenum)) {
		    /* Find the rank */
		    for (i = xlev_to_rank(mtmp->m_lev); i >= 0; i--)
			if (role->rank[i].m) {
			    /* Only need alternate message if no female form */
			    alt_mesg = !role->rank[i].f;
			    break;
			}
		}
	    }
	} else if (msg && is_were(olddata) &&
		monsndx(mdat) == counter_were(monsndx(olddata)))
	    alt_mesg = TRUE;

	/* WAC - At this point,  the transformation is going to happen */
	/* Reset values, remove worm tails, change levels...etc. */

	if(mtmp->wormno) {			/* throw tail away */
		wormgone(mtmp);
		place_monster(mtmp, mtmp->mx, mtmp->my);
	}

	hpn = mtmp->mhp;
	hpd = (int)mtmp->m_lev*8;	/* The "level < 50" check was screwing up the chameleons. --Amy */
	if(!hpd) hpd = 4;

	mtmp->m_lev = adj_lev(mdat);		/* new monster level */

	mhp = (int)mtmp->m_lev*8;
	if(!mhp) mhp = 4;

	/* new hp: same fraction of max as before */
#ifndef LINT
	mtmp->mhp = (int)(((long)hpn*(long)mhp)/(long)hpd);
#endif
	if(mtmp->mhp < 0) mtmp->mhp = hpn;	/* overflow */
/* Unlikely but not impossible; a 1HD creature with 1HP that changes into a
   0HD creature will require this statement */
	if (!mtmp->mhp) mtmp->mhp = 1;

/* and the same for maximum hit points */
	hpn = mtmp->mhpmax;
#ifndef LINT
	mtmp->mhpmax = (int)(((long)hpn*(long)mhp)/(long)hpd);
#endif
	if(mtmp->mhpmax < 0) mtmp->mhpmax = hpn;	/* overflow */
	if (!mtmp->mhpmax) mtmp->mhpmax = 1;

	/* take on the new form... */
	set_mon_data(mtmp, mdat, 0);

	if (emits_light(olddata) != emits_light(mtmp->data)) {
	    /* used to give light, now doesn't, or vice versa,
	       or light's range has changed */
	    if (emits_light(olddata))
		del_light_source(LS_MONSTER, (void *)mtmp);
	    if (emits_light(mtmp->data))
		new_light_source(mtmp->mx, mtmp->my, emits_light(mtmp->data),
				 LS_MONSTER, (void *)mtmp);
	}
	if (!mtmp->perminvis || pm_invisible(olddata))
	    mtmp->perminvis = pm_invisible(mdat);
	mtmp->minvis = mtmp->invis_blkd ? 0 : mtmp->perminvis;
	if (!(hides_under(mdat) && OBJ_AT(mtmp->mx, mtmp->my)) &&
			!(mdat->mlet == S_EEL && is_waterypool(mtmp->mx, mtmp->my)))
		mtmp->mundetected = 0;
	if (u.usteed) {
	    if (touch_petrifies(u.usteed->data) &&
	    		!Stone_resistance && rnl(3)) {
	    	char buf[BUFSZ];

	    	pline("You touch %s.", mon_nam(u.usteed));
	    	sprintf(buf, "riding %s", an(u.usteed->data->mname));
	    	instapetrify(buf);
 	    }
	    if (!can_ride(u.usteed)) dismount_steed(DISMOUNT_POLY);
	}

#ifndef DCC30_BUG
	if (mdat == &mons[PM_LONG_WORM] && (mtmp->wormno = get_wormno()) != 0) {
#else
	/* DICE 3.0 doesn't like assigning and comparing mtmp->wormno in the
	 * same expression.
	 */
	if (mdat == &mons[PM_LONG_WORM] &&
		(mtmp->wormno = get_wormno(), mtmp->wormno != 0)) {
#endif
	    /* we can now create worms with tails - 11/91 */
	    initworm(mtmp, rn2(5));
	    if (count_wsegs(mtmp))
		place_worm_tail_randomly(mtmp, mtmp->mx, mtmp->my);
	}

	newsym(mtmp->mx,mtmp->my);

	if (msg && (u.uswallow && mtmp == u.ustuck || canspotmon(mtmp))) {
	    if (alt_mesg && is_mplayer(mdat))
		pline("%s is suddenly very %s!", oldname,
			mtmp->female ? "feminine" : "masculine");
	    else if (alt_mesg)
		pline("%s changes into a %s!", oldname,
			is_human(mdat) ? "human" : mdat->mname + 4);
	    else {
	    uchar save_mnamelth = mtmp->mnamelth;
	    mtmp->mnamelth = 0;
	    pline("%s turns into %s!", oldname,
		  mdat == &mons[PM_GREEN_SLIME] ? "slime" :
		  x_monnam(mtmp, ARTICLE_A, (char*)0, SUPPRESS_SADDLE, FALSE));
	    mtmp->mnamelth = save_mnamelth;
	    }
	} else if (msg && couldsee)
	    /* No message if we only sensed the monster previously */
	    pline("%s suddenly disappears!", oldname);

	/* [ALI] In Slash'EM, this must come _after_ "<mon> turns into <mon>"
	 * since it's possible to get both messages.
	 */
	if (u.ustuck == mtmp) {
		if(u.uswallow) {
			if(!attacktype(mdat,AT_ENGL)) {
				/* Does mdat care? */
				if (!noncorporeal(mdat) && !amorphous(mdat) &&
				    !is_whirly(mdat) &&
				    (mdat != &mons[PM_YELLOW_LIGHT])) {
					You("break out of %s%s!", mon_nam(mtmp),
					    (is_animal(mdat)?
					    "'s stomach" : ""));
					mtmp->mhp = 1;  /* almost dead */
				}
				expels(mtmp, olddata, FALSE);
			} else {
				/* update swallow glyphs for new monster */
				swallowed(0);
			}
		} else if (!sticks(mdat) && !sticks(youmonst.data))
			unstuck(mtmp);
	}

	possibly_unwield(mtmp, polyspot);	/* might lose use of weapon */
	mon_break_armor(mtmp, polyspot);
	if (!(mtmp->misc_worn_check & W_ARMG))
	    mselftouch(mtmp, "No longer petrify-resistant, ",
			!flags.mon_moving);
	m_dowear(mtmp, FALSE);

	/* This ought to re-test can_carry() on each item in the inventory
	 * rather than just checking ex-giants & boulders, but that'd be
	 * pretty expensive to perform.  If implemented, then perhaps
	 * minvent should be sorted in order to drop heaviest items first.
	 */
	/* former giants can't continue carrying boulders */
	if (mtmp->minvent && !throws_rocks(mdat)) {
	    register struct obj *otmp, *otmp2;

	    for (otmp = mtmp->minvent; otmp; otmp = otmp2) {
		otmp2 = otmp->nobj;
		if (otmp->otyp == BOULDER) {
		    /* this keeps otmp from being polymorphed in the
		       same zap that the monster that held it is polymorphed */
		    if (polyspot) bypass_obj(otmp);
		    obj_extract_self(otmp);
		    /* probably ought to give some "drop" message here */
		    if (flooreffects(otmp, mtmp->mx, mtmp->my, "")) continue;
		    place_object(otmp, mtmp->mx, mtmp->my);
		}
	    }
	}

	return(1);
}

/* sometimes an egg will be special */
#define BREEDER_EGG (!rn2(77))

/*
 * Determine if the given monster number can be hatched from an egg.
 * Return the monster number to use as the egg's corpsenm.  Return
 * NON_PM if the given monster can't be hatched.
 */
int
can_be_hatched(mnum)
int mnum;
{
    /* ranger quest nemesis has the oviparous bit set, making it
       be possible to wish for eggs of that unique monster; turn
       such into ordinary eggs rather than forbidding them outright */
    if (mnum == PM_SCORPIUS) mnum = PM_SCORPION;

    mnum = little_to_big(mnum);
    /*
     * Queen bees lay killer bee eggs (usually), but killer bees don't
     * grow into queen bees.  Ditto for [winged-]gargoyles.
     */
    if (mnum == PM_KILLER_BEE || mnum == PM_GARGOYLE ||
	    (lays_eggs(&mons[mnum]) && (BREEDER_EGG ||
		(mnum != PM_QUEEN_BEE && mnum != PM_WINGED_GARGOYLE))))
	return mnum;
    return NON_PM;
}

/* type of egg laid by #sit; usually matches parent */
int
egg_type_from_parent(mnum, force_ordinary)
int mnum;	/* parent monster; caller must handle lays_eggs() check */
boolean force_ordinary;
{
    if (force_ordinary || !BREEDER_EGG) {
	if (mnum == PM_QUEEN_BEE) mnum = PM_KILLER_BEE;
	else if (mnum == PM_WINGED_GARGOYLE) mnum = PM_GARGOYLE;
    }
    return mnum;
}

/* decide whether an egg of the indicated monster type is viable; */
/* also used to determine whether an egg or tin can be created... */
boolean
dead_species(m_idx, egg)
int m_idx;
boolean egg;
{
	/*
	 * For monsters with both baby and adult forms, genociding either
	 * form kills all eggs of that monster.  Monsters with more than
	 * two forms (small->large->giant mimics) are more or less ignored;
	 * fortunately, none of them have eggs.  Species extinction due to
	 * overpopulation does not kill eggs.
	 */
	return (boolean)
		(m_idx >= LOW_PM &&
		 ((mvitals[m_idx].mvflags & G_GENOD) != 0 ||
		  (egg &&
		   (mvitals[big_to_little(m_idx)].mvflags & G_GENOD) != 0)));
}

/* kill off any eggs of genocided monsters */
STATIC_OVL void
kill_eggs(obj_list)
struct obj *obj_list;
{
	struct obj *otmp;

	for (otmp = obj_list; otmp; otmp = otmp->nobj)
	    if (otmp->otyp == EGG) {
		if (dead_species(otmp->corpsenm, TRUE)) {
		    /*
		     * It seems we could also just catch this when
		     * it attempted to hatch, so we wouldn't have to
		     * search all of the objlists.. or stop all
		     * hatch timers based on a corpsenm.
		     */
		    kill_egg(otmp);
		}
#if 0	/* not used */
	    } else if (otmp->otyp == TIN) {
		if (dead_species(otmp->corpsenm, FALSE))
		    otmp->corpsenm = NON_PM;	/* empty tin */
	    } else if (otmp->otyp == CORPSE) {
		if (dead_species(otmp->corpsenm, FALSE))
		    ;		/* not yet implemented... */
#endif
	    } else if (Has_contents(otmp)) {
		kill_eggs(otmp->cobj);
	    }
}

/* kill all members of genocided species */
void
kill_genocided_monsters()
{
	struct monst *mtmp, *mtmp2;
	boolean kill_cham[CHAM_MAX_INDX+1];
	int mndx;

	kill_cham[CHAM_ORDINARY] = FALSE;	/* (this is mndx==0) */
	for (mndx = 1; mndx <= CHAM_MAX_INDX; mndx++)
	  kill_cham[mndx] = (mvitals[cham_to_pm[mndx]].mvflags & G_GENOD) != 0;
	/*
	 * Called during genocide, and again upon level change.  The latter
	 * catches up with any migrating monsters as they finally arrive at
	 * their intended destinations, so possessions get deposited there.
	 *
	 * Chameleon handling:
	 *	1) if chameleons have been genocided, destroy them
	 *	   regardless of current form;
	 *	2) otherwise, force every chameleon which is imitating
	 *	   any genocided species to take on a new form.
	 */
	for (mtmp = fmon; mtmp; mtmp = mtmp2) {
	    mtmp2 = mtmp->nmon;
	    if (DEADMONSTER(mtmp)) continue;
	    mndx = monsndx(mtmp->data);
	    if ((mvitals[mndx].mvflags & G_GENOD) || kill_cham[mtmp->cham]) {
		if (mtmp->cham && !kill_cham[mtmp->cham])
		    /* [ALI] Chameleons are not normally subject to
		     * system shock, but genocide is a special case.
		     */
		    (void) mon_spec_poly(mtmp, (struct permonst *)0, 0L,
			    FALSE, FALSE, TRUE, TRUE);
		else
		    mondead(mtmp);
	    }
	    if (mtmp->minvent) kill_eggs(mtmp->minvent);
	}

	kill_eggs(invent);
	kill_eggs(fobj);
	kill_eggs(level.buriedobjlist);
}



#endif /* OVL2 */
#ifdef OVLB

void
golemeffects(mon, damtype, dam)
register struct monst *mon;
int damtype, dam;
{
	int heal=0, slow=0;

    if (mon->data == &mons[PM_FLESH_GOLEM]) {
	if (damtype == AD_ELEC) heal = dam / 6;
	else if (damtype == AD_FIRE || damtype == AD_COLD) slow = 1;
    } else if (mon->data == &mons[PM_IRON_GOLEM]) {
	if (damtype == AD_ELEC) slow = 1;
	else if (damtype == AD_FIRE) heal = dam;
    } else {
	return;
    }
    if (slow) {
	if (mon->mspeed != MSLOW)
	    mon_adjust_speed(mon, -1, (struct obj *)0);
    }
    if (heal) {
	if (mon->mhp < mon->mhpmax) {
	    mon->mhp += dam;
	    if (mon->mhp > mon->mhpmax) mon->mhp = mon->mhpmax;
	    if (cansee(mon->mx, mon->my))
		pline("%s seems healthier.", Monnam(mon));
	}
    }
}

boolean
angry_guards(silent)
register boolean silent;
{
	register struct monst *mtmp;
	register int ct = 0, nct = 0, sct = 0, slct = 0;
	int copcnt;

	for(mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
		if (DEADMONSTER(mtmp)) continue;
		if((mtmp->data == &mons[PM_WATCHMAN] || mtmp->data == &mons[PM_WATCH_LIEUTENANT] ||
			       mtmp->data == &mons[PM_WATCH_CAPTAIN] || mtmp->data == &mons[PM_WATCH_LEADER])
					&& mtmp->mpeaceful) {
			ct++;
			if(cansee(mtmp->mx, mtmp->my) && mtmp->mcanmove) {
				if (distu(mtmp->mx, mtmp->my) == 2) nct++;
				else sct++;
			}
			if (mtmp->msleeping || mtmp->mfrozen) {
				slct++;
				mtmp->msleeping = mtmp->mfrozen = 0;
			}
			mtmp->mpeaceful = 0;
		}
	}
	if(ct) {
	    if(!silent) { /* do we want pline msgs? */
		if(slct) pline_The("guard%s wake%s up!",
				 slct > 1 ? "s" : "", slct == 1 ? "s" : "");
		if(nct || sct) {
			if(nct) pline_The("guard%s get%s angry!",
				nct == 1 ? "" : "s", nct == 1 ? "s" : "");
			else if(!Blind)
				You("see %sangry guard%s approaching!",
				  sct == 1 ? "an " : "", sct > 1 ? "s" : "");
		} else if(flags.soundok)
			You_hear("the shrill sound of a guard's whistle.");
			if (PlayerHearsSoundEffects) pline(issoviet ? "Davayte posmotrim, sposobna li ubezhat' ot chasov v obshchey slozhnosti neudachnik! Vozmozhno net!" : "Pfiiiiiiiiiie!");
	    }

		copcnt = rnd(monster_difficulty() ) + 1;
		if (Role_if(PM_CAMPERSTRIKER)) copcnt *= (rn2(5) ? 2 : rn2(5) ? 3 : 5);

		if (uarmh && OBJ_DESCR(objects[uarmh->otyp]) && ( !strcmp(OBJ_DESCR(objects[uarmh->otyp]), "anti-government helmet") || !strcmp(OBJ_DESCR(objects[uarmh->otyp]), "antipravitel'stvennaya shlem") || !strcmp(OBJ_DESCR(objects[uarmh->otyp]), "aksil-hukumat dubulg'a") ) ) {
			copcnt = (copcnt / 2) + 1;
		}

		if (RngeAntiGovernment) {
			copcnt = (copcnt / 2) + 1;
		}

	      while(--copcnt >= 0) {
			(void) makemon(mkclass(S_KOP,0), u.ux, u.uy, MM_ANGRY);

			if (!rn2(10)) {

				int koptryct = 0;
				int kox, koy;

				for (koptryct = 0; koptryct < 2000; koptryct++) {
					kox = rn1(COLNO-3,2);
					koy = rn2(ROWNO);

					if (kox && koy && isok(kox, koy) && (levl[kox][koy].typ > DBWALL) && !(t_at(kox, koy)) ) {
						(void) maketrap(kox, koy, KOP_CUBE, 0);
						break;
						}
				}

			}

		} /* while */

	    return(TRUE);
	}
	return(FALSE);
}

void
pacify_guards()
{
	register struct monst *mtmp;

	for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
	    if (DEADMONSTER(mtmp)) continue;
	    if (mtmp->data == &mons[PM_WATCHMAN] || mtmp->data == &mons[PM_WATCH_LIEUTENANT] ||
		mtmp->data == &mons[PM_WATCH_CAPTAIN] || mtmp->data == &mons[PM_WATCH_LEADER])
	    mtmp->mpeaceful = 1;
	}
}

boolean
damage_mon(mon,amount,type)
struct monst* mon;
int amount;
int type;
{
	mon->mhp -= amount;
	return (mon->mhp < 1);
}

void
mimic_hit_msg(mtmp, otyp)
struct monst *mtmp;
short otyp;
{
	short ap = mtmp->mappearance;

	switch(mtmp->m_ap_type) {
	    case M_AP_NOTHING:			
	    case M_AP_FURNITURE:
	    case M_AP_MONSTER:
		break;
	    case M_AP_OBJECT:
		if (otyp == SPE_HEALING || otyp == SPE_EXTRA_HEALING || otyp == SPE_FULL_HEALING) {
		    pline("%s seems a more vivid %s than before.",
				The(simple_typename(ap)),
				c_obj_colors[objects[ap].oc_color]);
		}
		break;
	}
}
#endif /* OVLB */

/*mon.c*/
