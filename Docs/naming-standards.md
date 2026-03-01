# Folder Structure

All assets should follow this standardized hierarchy inside the project `Content` directory.

```
Content/
│
├── Art/
│   ├── Characters/
│   │   └── CharacterName/
│   │
│   ├── Environments/
│   │   ├── Biomes/
│   │   ├── Buildings/
│   │   └── Props/
│   │
│   ├── Materials/
│   │
│   ├── Textures/
│   │
│   ├── FX/
│   │
│   └── Decals/
│
├── Blueprints/
│
├── UI/
│
└── Audio/

```

### Environment Example

```
Art/Environments/Props/Industrial/
Art/Environments/Buildings/Warehouse/
Art/Environments/Biomes/Desert/

```

### Character Example

```
Art/Characters/Hero/
Art/Characters/NPC/
Art/Characters/Enemies/

```

---

# Asset Naming Format

All assets should follow the format:

```
Prefix_AssetName_Variant_Suffix

```

Example:

```
SM_Rock_Cliff_A

```

or

```
T_Rock_Cliff_N

```

### Format Breakdown

| Component | Meaning | Example |
| --- | --- | --- |
| Prefix | Asset type identifier | SM |
| AssetName | Descriptive object name | RockCliff |
| Variant | Optional variation | A |
| Suffix | Texture type or additional identifier | N |

---

# Asset Prefix Standards

| Prefix | Asset Type |
| --- | --- |
| SM | Static Mesh |
| SK | Skeletal Mesh |
| M | Material |
| MI | Material Instance |
| T | Texture |
| BP | Blueprint |
| FX | Particle / VFX |
| D | Decal |
| AN | Animation |
| S | Sound |
| UI | UI Element |

---

# Texture Suffix Standards

| Suffix | Meaning |
| --- | --- |
| _D | Diffuse / BaseColor |
| _N | Normal |
| _R | Roughness |
| _M | Metallic |
| _AO | Ambient Occlusion |
| _ORM | Occlusion Roughness Metallic |
| _E | Emissive |
| _H | Height |

Example:

```
T_Rock_Cliff_D
T_Rock_Cliff_N
T_Rock_Cliff_ORM

```

---

# Variant Naming

Variants should use alphabetical or numerical identifiers.

Examples:

```
SM_Rock_Cliff_A
SM_Rock_Cliff_B
SM_Rock_Cliff_C

```

or

```
SM_WoodCrate_01
SM_WoodCrate_02
SM_WoodCrate_03

```

---

# Material Naming

Materials and instances follow this format:

```
M_AssetName
MI_AssetName_Variant

```

Examples:

```
M_Rock_Cliff
MI_Rock_Cliff_Wet
MI_Rock_Cliff_Sandy

```

---

# Blueprint Naming

Blueprints should clearly indicate their purpose.

Examples:

```
BP_Door_Industrial
BP_Light_Flicker
BP_Enemy_Soldier

```

---

# Example Asset Renaming

### Before

```
rock01
Rock_Texture
crate_final_v3
metal_mat

```

### After

```
SM_Rock_Cliff_A
T_Rock_Cliff_D
SM_WoodCrate_01
M_Metal_Industrial

```

---

# Example Asset Group

Example asset set for a cliff rock:

```
SM_Rock_Cliff_A
M_Rock_Cliff
MI_Rock_Cliff_Wet

T_Rock_Cliff_D
T_Rock_Cliff_N
T_Rock_Cliff_ORM

```

Folder:

```
Art/Environments/Props/Rocks/

```

---

# General Guidelines

### Naming Rules

- Use **PascalCase** for asset names
- Avoid spaces
- Avoid special characters
- Use clear, descriptive names

### Do

```
SM_StoneWall_A
T_StoneWall_N
MI_StoneWall_Wet

```

### Avoid

```
stone_wall_new
wallFINAL
rockTexture

```
